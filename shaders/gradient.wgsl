

@group(0) @binding(0) var tex: texture_storage_2d<rgba8unorm, write>;


struct Uniforms {
    resolution: vec2<i32>,
}

const PI: f32 = 3.14159265359;
const TWO_PI: f32 = 6.28318530718;

struct In {
    @builtin(global_invocation_id)  gid: vec3<u32>,
}


fn get_angle(uv: vec2<f32>) -> f32 {
    let normalized = normalize(uv);

    let angle = acos(dot(normalized, vec2<f32>(1.0, 0.0)));

    return select(angle, TWO_PI - angle, normalized.y < 0.0);
}



@compute @workgroup_size(1)
fn main(in: In) {
    let resolution = vec2<i32>(1920, 1080);

    let uv = (((vec2<f32>(in.gid.xy) / vec2<f32>(resolution)) * 2.0) - 1.0) * vec2<f32>(1.0, -1.0 / (f32(resolution.x) / f32(resolution.y)));

    var color = vec3<f32>(0.0, 0.0, 0.0);

    let length = length(uv);
    let angle = get_angle(uv) + 0.5;
    let in_circle = select(false, true, length(uv) < 0.41 && length(uv) > 0.4);

    let in_valid_angle = select(false, true, i32(angle / (TWO_PI / 30.0)) % 2 == 0);

    color += select(vec3<f32>(1.0, 1.0, 1.0), vec3<f32>(0.0, 0.0, 0.0), in_circle && in_valid_angle);

    textureStore(tex, vec2<i32>(in.gid.xy), vec4<f32>(color, 1.0));
}