    class Metal : public Material
    {
      public:
        Metal(const Vector3D& a) : albedo(a) {}

        virtual bool scatter(const Ray& r_in, const HitInfo& rec,
                             Vector3D& attenuation,
                             Ray& scattered) const override
        {
            Vector3D reflected = reflect(unit_vector(r_in.direction()),
                                         rec.normal);
            scattered = Ray(rec.hit_point, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

      private:
        Vector3D reflect(const Vector3D& v, const Vector3D& n) const
        {
            return v - 2 * dot(v, n) * n;
        }

      public:
        Vector3D albedo;
    };