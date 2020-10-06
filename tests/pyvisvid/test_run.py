import os
import pytest
from pyvisvid import run


@pytest.fixture()
def sample_destination(tmp_path):
    sample_destination_path = tmp_path / "dummy"
    return str(sample_destination_path)


def test_is_valid_output_valid(sample_destination):
    assert run.is_valid_output(os.path.join(sample_destination, "output.pgm")) is True


def test_is_valid_output_invalid(sample_destination):
    assert run.is_valid_output(os.path.join(sample_destination, "output.txt")) is False
