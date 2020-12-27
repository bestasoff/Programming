import sys
import typing as tp
from pathlib import Path
import io


def tail(filename: Path, lines_amount: int = 10, output: tp.Optional[tp.IO[bytes]] = None) -> None:
    """
    :param filename: file to read lines from (the file can be very large)
    :param lines_amount: number of lines to read
    :param output: stream to write requested amount of last lines from file
                   (if nothing specified stdout will be used)
    """
    buf_size: int = 512
    file_size: int = filename.stat().st_size
    if buf_size > file_size:
        buf_size = file_size

    if file_size == 0 or lines_amount == 0:
        if output:
            output.write(b"")
        else:
            sys.stdout.write("")
        return

    lines_read: int = 0
    hit_end_of_file: bool = False
    prev_pos: int = 0
    read_pos: int = 0
    with filename.open("rb") as file:
        file.seek(0, io.SEEK_END)
        while read_pos != file_size and lines_read < lines_amount + 1:
            if prev_pos + buf_size > file_size:
                hit_end_of_file = True
            read_pos = prev_pos + buf_size if not hit_end_of_file else file_size
            file.seek(-read_pos, 2)
            lines_read += file.read(read_pos - prev_pos).count(b"\n")
            prev_pos = read_pos
        file.seek(-read_pos, io.SEEK_END)
        buffer: bytes = file.read()
        endl_pos: int = buffer.find(b"\n") if lines_read > lines_amount else -1
        while lines_read - 1 > lines_amount:
            endl_pos = buffer.find(b"\n", endl_pos + 1)
            lines_read -= 1
        if output:
            output.write(memoryview(buffer)[endl_pos + 1:])
        else:
            sys.stdout.write(buffer[endl_pos + 1:].decode())
    return
