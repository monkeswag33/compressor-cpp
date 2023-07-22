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
    b.installArtifact(compressor);
    var compressor_cmd = b.addRunArtifact(compressor);
    compressor_cmd.step.dependOn(b.getInstallStep());

    var decompressor = b.addExecutable(.{
        .name = "decompressor",
        .root_source_file = .{ .path = "src/decompressor.zig" },
        .target = target,
        .optimize = optimize
    });
    b.installArtifact(decompressor);
    var decompressor_cmd = b.addRunArtifact(decompressor);
    decompressor_cmd.step.dependOn(b.getInstallStep());

    if (b.args) |args| {
        compressor_cmd.addArgs(args);
        decompressor_cmd.addArgs(args);
    }
    const compressor_run = b.step("run-compressor", "Run compressor");
    const decompressor_run = b.step("run-decompressor", "Run decompressor");
    compressor_run.dependOn(&compressor_cmd.step);
    decompressor_run.dependOn(&decompressor_cmd.step);
}
