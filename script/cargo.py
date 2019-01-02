#!/usr/bin/env python

import argparse
import os
import sys
import subprocess
import shutil


def main():
    args = parse_args()

    is_debug = args.is_debug[0]
    manifest_path = args.manifest_path[0]
    build_path = args.build_path[0]

    build(is_debug, manifest_path, build_path)


def parse_args():
    parser = argparse.ArgumentParser(description='Cargo')

    parser.add_argument('--is_debug', nargs=1)
    parser.add_argument('--manifest_path', nargs=1)
    parser.add_argument('--build_path', nargs=1)

    args = parser.parse_args()

    # validate args
    if (args.is_debug is None or
        len(args.is_debug) is not 1 or
            len(args.is_debug[0]) is 0):
        raise Exception("is_debug argument was not specified correctly")

    if (args.is_debug[0] != "false" and args.is_debug[0] != "true"):
        raise Exception("is_debug argument was not specified correctly")

    if (args.manifest_path is None or
        len(args.manifest_path) is not 1 or
            len(args.manifest_path[0]) is 0):
        raise Exception("manifest_path argument was not specified correctly")

    if (args.build_path is None or
        len(args.build_path) is not 1 or
            len(args.build_path[0]) is 0):
        raise Exception("build_path argument was not specified correctly")

    if "out" not in args.build_path[0]:
        raise Exception("build_path did not contain 'out'")

    # args are valid
    return args


def get_platform():
    platforms = {
        'win32' : 'Windows',
        'darwin' : 'macOS',
        'linux1' : 'Linux',
        'linux2' : 'Linux'
    }

    if sys.platform not in platforms:
        return sys.platform

    return platforms[sys.platform]


def build(is_debug, manifest_path, build_path):
    platform = get_platform()
    if platform == "Windows":
        target = "x86_64-pc-windows-msvc"
    elif platform == "macOS":
        target = "x86_64-apple-darwin"
    elif platform == "Linux":
        target = "x86_64-unknown-linux-musl"

    args = []
    args.append("cargo")
    args.append("build")
    if is_debug == "false":
        args.append("--release")
    args.append("--manifest-path=" + manifest_path)
    args.append("--target-dir=" + build_path)
    args.append("--target=" + target)

    env = os.environ.copy()
    env['NO_CXXEXCEPTIONS'] = "1"
    if is_debug == "false":
        env['NDEBUG'] = "1"

    try:
      subprocess.check_call(args, env=env)
    except subprocess.CalledProcessError as e:
      print e.output
      raise e


if __name__ == '__main__':
    sys.exit(main())
