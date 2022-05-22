#include <CIL/ImageInfo.hpp>
#include <CIL/Pixel.hpp>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)
#define UNUSED(x) (void) (x)

namespace CIL {
    class ThreadHandler
    {
        const int processor_count = // 1;
            (std::thread::hardware_concurrency()
                 ? std::thread::hardware_concurrency()
                 : 2);
        std::vector<std::thread> v;
        static int index;

      public:
        std::function<void(int, int)> fn;
        ThreadHandler()
        {
            std::cerr << "processor_count: " << processor_count << "\n";
            v.resize(processor_count);
        }
        void process_row(int row, int width)
        {
            assert(fn && "fn should not be empty!!");

            if (v[index].joinable())
                v[index].join();

            std::function<void()> fn2 = [this, row, width]() {
                for (auto i = 0; i < width; i++)
                    fn(row, i);
            };

            v[index++] = std::thread(fn2);
            index = index % processor_count;
        }

        void process_matrix(std::size_t width, std::size_t height)
        {
            assert(fn && "fn should not be empty!!");
            for (std::size_t i = 0; i < height; ++i)
            {
                std::cerr << "Processing row: " << i << "\n";
                process_row(i, width);
            }
            joinAll();
        }

        void joinAll()
        {
            for (int i = 0; i < processor_count; i++)
                if (v[i].joinable())
                    v[i].join();
        }
        ~ThreadHandler() { joinAll(); }
    };
} // namespace CIL
