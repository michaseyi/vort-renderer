


struct VertexIn {
    @location(0) position: vec3,
    @location(1) normal: vec3,
    @location(2) tex_coord: vec2,
    @location(3) tangent: vec3,
    @location(4) bitangent: vec3,
    @location(5) bone_weights: array<f32, 10>,
    @location(6) bone_ids: array<i32, 10>
}

