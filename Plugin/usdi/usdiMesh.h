#pragma once

namespace usdi {

union VAFlags
{
    uint32_t any = 0;
    struct {
        uint32_t points : 1;
        uint32_t normals : 1;
        uint32_t colors : 1;
        uint32_t uvs : 1;
        uint32_t tangents : 1;
        uint32_t velocities : 1;
        uint32_t weights : 1;
    };
};

struct SubmeshSample
{
    VtArray<GfVec3f> points;
    VtArray<GfVec3f> normals;
    VtArray<GfVec4f> colors;
    VtArray<GfVec2f> uvs;
    VtArray<GfVec4f> tangents;
    VtArray<GfVec3f> velocities;
    VtArray<int>     indices;
    VtArray<Weights4> weights4;
    VtArray<Weights8> weights8;
    float3           bounds_min = {}, bounds_max = {};
    float3           center = {}, extents = {};
};

struct MeshSample
{
    VtArray<GfVec3f> points;
    VtArray<GfVec3f> normals;
    VtArray<GfVec4f> colors;
    VtArray<GfVec2f> uv0, uv1;
    VtArray<GfVec4f> tangents;
    VtArray<GfVec3f> velocities;
    VtArray<int>     counts;
    VtArray<int>     offsets;
    VtArray<int>     indices;
    VtArray<int>     indices_triangulated;
    VtArray<int>     indices_flattened_triangulated;

    VtArray<float>      bone_weights;
    VtArray<int>        bone_indices;
    VtArray<GfMatrix4f> bindposes;
    VtArray<TfToken>    bones;
    VtArray<const char*> bones_;
    TfToken          root_bone;
    VtArray<Weights4> weights4;
    VtArray<Weights8> weights8;
    int              max_bone_weights = 4;

    float3           bounds_min = {}, bounds_max = {};
    float3           center = {}, extents = {};
};


class Mesh : public Xform
{
typedef Xform super;
public:
    DefSchemaTraits(UsdGeomMesh, "Mesh");

    Mesh(Context *ctx, Schema *parent, const UsdPrim& prim);
    Mesh(Context *ctx, Schema *parent, const char *name, const char *type = _getUsdTypeName());
    ~Mesh() override;

    void                updateSample(Time t) override;

    const MeshSummary&  getSummary() const;
    bool                readSample(MeshData& dst, Time t, bool copy);
    bool                writeSample(const MeshData& src, Time t);

    using SampleCallback = std::function<void(const MeshData& data, Time t)>;
    int eachSample(const SampleCallback& cb);

    // ugly workaround for C# (C# strings are need to be copied on C++ side)
    void                assignRootBone(MeshData& dst, const char *v);
    void                assignBones(MeshData& dst, const char **v, int n);

private:
    typedef std::vector<SubmeshSample> SubmeshSamples;

    UsdGeomMesh         m_mesh;
    MeshSample          m_sample;
    SubmeshSamples      m_submeshes;
    Attribute           *m_attr_colors = nullptr;
    Attribute           *m_attr_uv0 = nullptr;

    // bone & weights attributes
    Attribute           *m_attr_bone_weights = nullptr;
    Attribute           *m_attr_bone_indices = nullptr;
    Attribute           *m_attr_bindposes = nullptr;
    Attribute           *m_attr_bones = nullptr;
    Attribute           *m_attr_root_bone = nullptr;
    Attribute           *m_attr_max_bone_weights = nullptr;

    mutable bool        m_summary_needs_update = true;
    mutable MeshSummary m_summary;
    int                 m_num_indices = 0;
    int                 m_num_indices_triangulated = 0;
    int                 m_num_current_submeshes = 0;
};

} // namespace usdi
