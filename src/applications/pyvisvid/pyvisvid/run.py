import os
import argparse
import sys

from . import visvid


def is_valid_source(source):

    if not os.path.exists(source):
        return False

    if not os.path.isfile(source):
        return False

    return True


def is_valid_output(destination):
    if os.path.isdir(destination):
        return False

    ext = os.path.splitext(os.path.basename(destination))[-1]

    if ext != ".pgm":
        return False

    return True


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("source")
    parser.add_argument("destination")

    args = parser.parse_args()

    if not is_valid_source(args.source):
        print("Invalid source: {}".format(args.source))
        sys.exit(1)

    if not is_valid_output(args.destination):
        print("Invalid output: {}".format(args.destination))
        sys.exit(1)

    print("Using visvid version {}".format(visvid.get_version()))
    print("Processing {}".format(args.source))
    visvid.make_pgm(args.source, args.destination)

    if os.path.exists(args.destination):
        print("Saved as {}".format(args.destination))
    else:
        print("Process failed")


if __name__ == '__main__':
    main()
