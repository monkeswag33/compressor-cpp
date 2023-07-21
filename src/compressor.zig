const compress_dir = @import("compression.zig").compressDir;
const std = @import("std");
const log = std.log;
const fs = std.fs;
const path = fs.path;
const fmt = std.fmt;

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();
    defer _ = gpa.deinit();
    var args = std.process.args();
    _ = args.next();
    var arg = args.next();
    if (arg == null) {
        log.err("Please pass the name of the directory to compress", .{});
        std.process.exit(1);
    }
    var dir_name = arg.?;
    const real_path = fs.cwd().realpathAlloc(allocator, dir_name) catch |err| {
        if (err == error.FileNotFound) {
            log.err("Could not find directory", .{});
            std.process.exit(1);
        }
        return err;
    };
    defer allocator.free(real_path);
    const filename = try fmt.allocPrint(allocator, "{s}.cmp", .{path.basename(dir_name)});
    defer allocator.free(filename);
    var file = try fs.cwd().createFile(filename, .{});
    defer file.close();
    try compress_dir(allocator, real_path, &file);
}
