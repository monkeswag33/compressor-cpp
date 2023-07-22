const std = @import("std");
const Build = @import("std").Build;

pub fn build(b: *Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    var compressor = b.addExecutable(.{
        .name = "compressor",
        .root_source_file = .{ .path = "src/compressor.zig" },
        .target = target,
        .optimize = optimize
    });

    var decompressor = b.addExecutable(.{
        .name = "decompressor",
        .root_source_file = .{ .path = "src/decompressor.zig" },
        .target = target,
        .optimize = optimize
    });

    var compressor_cmd = (b.addRunArtifact(compressor));
    var decompressor_cmd = (b.addRunArtifact(decompressor));
    if (b.args != null) {
        compressor_cmd.addArgs(b.args.?);
        decompressor_cmd.addArgs(b.args.?);
    }
    const compressor_run = b.step("run-compressor", "Run compressor");
    const decompressor_run = b.step("run-decompressor", "Run decompressor");
    compressor_run.dependOn(&compressor_cmd.step);
    decompressor_run.dependOn(&decompressor_cmd.step);
    b.installArtifact(compressor);
    b.installArtifact(decompressor);
    b.default_step.dependOn(&compressor.step);
    b.default_step.dependOn(&decompressor.step);
}
