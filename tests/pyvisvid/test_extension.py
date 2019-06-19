import os
import pytest
from pyvisvid import visvid
import shutil
import subprocess
import struct

@pytest.fixture
def generated_pattern(tmpdir_factory):
    ffmpeg = shutil.which("ffmpeg")
    assert ffmpeg, "Ffmpeg required for test"
    my_path =  tmpdir_factory.mktemp("source", numbered=False)
    test_file_length = "5.3"
    src =  os.path.join(my_path, "testsrc2.mp4")
    if not os.path.exists(src):
        create_test_file_command = [
            ffmpeg,
            "-f", "lavfi",
            "-i", f"testsrc=duration={test_file_length}:size=qcif:rate=10",
            src
        ]
        subprocess.check_call(create_test_file_command)

    assert os.path.exists(src), f"Generating test file {src} was unsuccessful"

    yield src
    shutil.rmtree(my_path)


def test_create_pgm(generated_pattern, tmpdir_factory):

    pgm_out = "output.pgm"
    my_path = tmpdir_factory.mktemp("output", numbered=False)
    resulting_file = os.path.join(my_path, pgm_out)

    assert visvid.make_pgm(generated_pattern, output=resulting_file) == 0
    with open(resulting_file, "rb") as f:
        assert os.path.exists(resulting_file), f"{resulting_file} does not exist"
        magic_number = f.readline().strip()

        #     The magic number for PGM files is P5
        assert magic_number == b"P5", f"magic_number = {magic_number}"
    shutil.rmtree(my_path)


def test_pgm_size(generated_pattern, tmpdir_factory):
    expected_width = 176
    expected_height = 51

    pgm_out = "output1.pgm"
    my_path =  tmpdir_factory.mktemp("outputA", numbered=False)
    resulting_file = os.path.join(my_path, pgm_out)
    assert visvid.make_pgm(generated_pattern, resulting_file) == 0;
    with open(resulting_file, "rb") as f:
        f.readline()

        # The resolution in the pgm format is on the second line, values in
        # ASCII sep by a null and end with a newline
        resolution = f.readline().strip()
        actual_width, actual_height = resolution.split()

    assert int(actual_width) == expected_width, \
        f"actual width is {actual_width}, expected {expected_width}"

    assert int(actual_height) == expected_height, \
        f"actual height is {actual_height}, expected {expected_height}"
    shutil.rmtree(my_path)


def test_version():
    assert visvid.get_version() == "0.1"