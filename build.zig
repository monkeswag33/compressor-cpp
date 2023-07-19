const Build = @import("std").Build;
const Pkg = @import("std").build;

pub fn build(b: *Build) void {
    const exe = b.addExecutable(.{ .name = "compressor", .root_source_file = .{ .path = "src/compressor.zig" } });
    b.installArtifact(exe);
}
