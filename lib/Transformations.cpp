#include <CIL/Core/Coordinate.hpp>
#include <CIL/Core/Debug.hpp>
#include <CIL/Core/DetachedFPPixel.hpp>
#include <CIL/Core/Types.hpp>
#include <CIL/Core/Utils.hpp>
#include <CIL/ImageInfo.hpp>
#include <CIL/ThreadHandler.hpp>
#include <CIL/Transformations.hpp>
#include <cassert>
#include <cmath>
#include <limits>

namespace CIL {
    void invertColor(ImageInfo& img)
    {
        CIL::ThreadHandler th;
        th.fn = [&img](int r, int c) {
            for (auto i = 0U; i < img.numComponents(/*count_alpha=*/false); i++)
                img(r, c, i) = CIL::Pixel::max() - img(r, c, i);
        };
        th.process_matrix(img.width(), img.height());
    }

    void changeContrast(ImageInfo& img, const double contrast)
    {
        ThreadHandler th;
        double temp = 259 * (contrast + 255) / (255 * (259 - contrast));
        th.fn = [&](int r, int c) {
            auto px = img(r, c);
            for (auto i = 0U; i < img.numComponents(/*count_alpha=*/false); i++)
            {
                double ans = temp * (px[i] - 128) + 128;
                px[i] = (ans > CIL::Pixel::max())
                            ? CIL::Pixel::max()
                            : ((ans < 0) ? 0 : round(ans));
            }
        };
        th.process_matrix(img.width(), img.height());
    }

    void changeGamma(ImageInfo& img, const double samples_per_pixel)
    {
        ThreadHandler th;
        th.fn = [&](int r, int c) {
            auto px = img(r, c);
            // Divide the color by the number of samples and gamma-correct for
            // gamma=2.0.
            auto scale = 255.0 / samples_per_pixel;
            for (auto i = 0U; i < img.numComponents(/*count_alpha=*/false); i++)
                px[i] = sqrt(scale * px[i]);
        };
        th.process_matrix(img.width(), img.height());
    }

    void changeBrightness(ImageInfo& img, int16_t brightness)
    {
        ThreadHandler th;
        th.fn = [&](int r, int c) {
            auto px = img(r, c);
            DetachedFPPixel dpx(px);

            dpx += brightness;
            dpx.capRange(0, 255);

            if (img.hasAlphaComponent())
                dpx.back() = px.back();

            px = dpx;
        };
        th.process_matrix(img.width(), img.height());
    }

    void flipImage(ImageInfo& img, const Axis axis)
    {
        ThreadHandler th;
        uint32_t outer, inner;

        outer = (axis == Axis::Y) ? img.height() : img.width();
        inner = (axis == Axis::Y) ? img.width() / 2 : img.height() / 2;

        th.fn = [&](int r, int c) {
            Pixel px1, px2;
            px1 = (axis == Axis::Y) ? img(r, c) : img(c, r);
            px2 = (axis == Axis::Y) ? img(r, img.width() - c - 1)
                                    : img(img.height() - c - 1, r);

            if (likely(px1.isValid() && px2.isValid()))
                Pixel::swap(px1, px2);
        };

        th.process_matrix(inner, outer);
    }

    void cropImage(ImageInfo& img, const Dimensions& dims)
    {
        ThreadHandler th;

        auto new_width = img.width() - dims.left - dims.right;
        auto new_height = img.height() - dims.top - dims.bottom;

        ImageMatrix new_img(new_width, new_height, img.numComponents(),
                            img.sampleDepth());

        th.fn = [&](int r, int c) {
            auto px1 = img(dims.top + r, dims.left + c);
            auto px2 = new_img(r, c);

            if (likely(px1.isValid() && px2.isValid()))
                px2.copyComponents(px1);
        };

        th.process_matrix(new_width, new_height);

        img.setData(new_img);
    }

    void padImage(ImageInfo& img, const Dimensions& dims)
    {
        ThreadHandler th;
        auto new_width = img.width() + dims.left + dims.right;
        auto new_height = img.height() + dims.top + dims.bottom;

        ImageMatrix new_img(new_width, new_height, img.numComponents(),
                            img.sampleDepth());

        th.fn = [&](int r, int c) {
            auto px1 = img(r, c);
            auto px2 = new_img(dims.top + r, dims.left + c);

            if (likely(px1.isValid() && px2.isValid()))
                px2.copyComponents(px1);
        };

        th.process_matrix(img.width(), img.height());
        img.setData(new_img);
    }

    static void createGaussianFilter(Sequence<double>& GKernel, int size)
    {
        // initialising standard deviation value to the size of kernel
        // to increase blur effect
        double sigma = size;
        double r, s = 2.0 * sigma * sigma;

        // sum is for normalization
        double sum = 0.0;

        GKernel.resize(size);
        const int temp = size / 2;
        // generating 5x5 kernel
        for (int x = -temp; x <= temp; x++)
        {
            GKernel[x + temp].resize(size);
            for (int y = -temp; y <= temp; y++)
            {
                r = sqrt(x * x + y * y);
                GKernel[x + temp][y + temp] = (exp(-(r * r) / s)) /
                                              (utils::pi * s);
                sum += GKernel[x + temp][y + temp];
            }
        }

        // normalising the Kernel
        for (auto i = 0U; i < GKernel.size(); ++i)
            for (auto j = 0U; j < GKernel[0].size(); ++j)
                GKernel[i][j] /= sum;
    }

    struct Kernel
    {
        const Sequence<double> matrix;
        const KernelType kernel_type;

        Kernel(const KernelType kernel_type, const Sequence<double>& matrix)
            : matrix(matrix), kernel_type(kernel_type)
        {}
        static Kernel get(const KernelType& kernel_type, int size = 3)
        {
            if (size == 0)
                assert("size can't be zero");

            switch (kernel_type)
            {
                case KernelType::BOX_BLUR:
                {
                    Sequence<double> matrix;
                    matrix.resize(size);
                    for (int i = 0; i < size; i++)
                    {
                        matrix[i].resize(size, 1.00);
                    }
                    utils::multiplyConstantToSequence(matrix,
                                                      1.00 / size * size);
                    return Kernel(kernel_type, matrix);
                }
                case KernelType::GAUSSIAN_BLUR:
                {
                    Sequence<double> matrix;
                    createGaussianFilter(matrix, size);
                    return Kernel(kernel_type, matrix);
                }
                case KernelType::EDGE_DETECTION:
                {
                    // TODO: need to find algorithm to expand it's size to
                    // different ones
                    Sequence<double> matrix{{-1, -1, -1},
                                            {-1, 8, -1},
                                            {-1, -1, -1}};
                    return Kernel(kernel_type, matrix);
                }
                default:
                {
                    assert(false);
                }
            }
        }
    };

    void applyKernel(ImageInfo& img, KernelType kernel_type, int size)
    {
        assert((size & 1) && "only odd sized kernels allowed");

        Kernel k = Kernel::get(kernel_type, size);

        // pad the image to get the output image of same size as input image
        padImage(img, Dimensions((k.matrix.size() - 1) / 2));

        ImageMatrix matrix = img.getData();
        matrix.convolute(k.matrix);
        img.setData(matrix);
    }

    // TODO: Add `preserve_image` parameter.
    void rotate(ImageInfo& img, int degrees, RotationKind rotation_kind)
    {
        ThreadHandler th;
        ImageMatrix new_image_data(img.width(), img.height(),
                                   img.numComponents(), img.sampleDepth(),
                                   nullptr, img.hasAlphaComponent());
        Coordinate origin((img.width() / 2) + 0.5, (img.height() / 2) + 0.5);

        th.fn = [&](int row, int col) {
            auto px = new_image_data(row, col);
            Coordinate P(px.col() + 0.5, px.row() + 0.5);
            auto source = utils::computeCoordinateAfterRotation(origin, P,
                                                                -degrees);
            if (rotation_kind == RotationKind::rotation_by_area_mapping)
            {
                px = utils::bilinearInterpolation(img, source);
            } else if (rotation_kind == RotationKind::rotation_by_sampling)
            {
                auto source_pixel = img(std::lround(source.y),
                                        std::lround(source.x));
                if (!source_pixel.isValid())
                    return;

                for (auto i = 0; i < px.numComponents(); ++i)
                {
                    px[i] = source_pixel[i];
                }
            }
        };

        th.process_matrix(new_image_data.width(), new_image_data.height());
        img.setData(new_image_data);
    }

    void convertToGrayscale(ImageInfo& img, bool preserve_colortype)
    {
        ThreadHandler th;
        if (img.colorModel() == ColorModel::COLOR_GRAY ||
            img.colorModel() == ColorModel::COLOR_GRAY_ALPHA)
            return;
        auto num_components = (preserve_colortype
                                   ? img.numComponents()
                                   : 1 + img.hasAlphaComponent());
        ImageMatrix new_img_data(img.width(), img.height(), num_components,
                                 img.sampleDepth());

        th.fn = [&](int r, int c) {
            auto px = new_img_data(r, c);
            DetachedFPPixel dpx = img(px.row(), px.col());
            dpx.scale({0.299, 0.587, 0.114});
            auto gray_value = std::lround(dpx.sum(/*exclude_alpha=*/true));
            dpx.forEach([gray_value](typename DetachedFPPixel::ValueType&
                                         val) { val = gray_value; },
                        /*exclude_alpha=*/true);
            px = dpx;
        };

        th.process_matrix(new_img_data.width(), new_img_data.height());

        if (!preserve_colortype)
        {
            if (img.hasAlphaComponent())
                img.setColorModel(ColorModel::COLOR_GRAY_ALPHA);
            else
                img.setColorModel(ColorModel::COLOR_GRAY);
        }
        img.setData(new_img_data);
    }

    void resize(ImageInfo& img, uint32_t new_width, uint32_t new_height,
                ResizeAlgorithm resize_algorithm)
    {
        ThreadHandler th;
        double scale_x = (new_width * 1.00) / img.width();
        double scale_y = (new_height * 1.00) / img.height();

        ImageMatrix new_image_data(new_width, new_height, img.numComponents(),
                                   img.sampleDepth());

        th.fn = [&](int row, int col) {
            auto px = new_image_data(row, col);
            Coordinate source = {px.col() / scale_x, px.row() / scale_y};
            switch (resize_algorithm)
            {
                case ResizeAlgorithm::nearest_neighbour_interpolation:
                {
                    auto old_px = img(source.getNearestIntegralY(),
                                      source.getNearestIntegralX());
                    if (old_px.isValid())
                        px.copyComponents(old_px);
                    break;
                }
                case ResizeAlgorithm::bilinear_interpolation:
                    px = utils::bilinearInterpolation(img, source);
                    break;
            }
        };

        th.process_matrix(new_image_data.width(), new_image_data.height());
        img.setData(new_image_data);
    }

    void zoom(ImageInfo& img, const Dimensions& dims)
    {
        auto width = img.width();
        auto height = img.height();
        cropImage(img, dims);
        resize(img, width, height);
    }
} // namespace CIL
