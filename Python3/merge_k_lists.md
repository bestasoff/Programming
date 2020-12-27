## Merge k lists problem

### Merge 2 lists

```python3
def merge(lst_a: tp.List[int], lst_b: tp.List[int]) -> tp.List[int]:
    """
    Merge two sorted lists in one sorted list
    :param lst_a: first sorted list
    :param lst_b: second sorted list
    :return: merged sorted list
    """
    i, j = 0, 0
    res = list()
    while i < len(lst_a) and j < len(lst_b):
        if lst_a[i] <= lst_b[j]:
            res.append(lst_a[i])
            i += 1
        else:
            res.append(lst_b[j])
            j += 1
    if i != len(lst_a):
        for i1 in range(i, len(lst_a)):
            res.append(lst_a[i1])
    elif j != len(lst_b):
        for j1 in range(j, len(lst_b)):
            res.append(lst_b[j1])
    return res

```
### Merge _k_ lists

```python3
def merge(seq: tp.Sequence[tp.Sequence[int]]) -> tp.List[int]:
    """
    :param seq: sequence of sorted sequences
    :return: merged sorted list
    """
    heap: tp.List[tp.List[int]] = [[arr[0], 0, i] for i, arr in enumerate(seq) if arr]
    result: tp.List[int] = []
    heapq.heapify(heap)

    if len(heap) == 0:
        return []

    while heap[0][0] != int(sys.maxsize):
        top: tp.List[int] = heap[0]
        result.append(top[0])
        top[1] += 1
        if top[1] >= len(seq[top[2]]):
            top[0] = int(sys.maxsize)
        else:
            top[0] = seq[top[2]][top[1]]
        heapq.heapify(heap)
    return result

```
### Merge _k_ streams

```python3
def merge(input_streams: tp.Sequence[tp.IO[bytes]], output_stream: tp.IO[bytes]) -> None:
    """
    Merge input_streams in output_stream
    :param input_streams: list of input streams. Contains byte-strings separated by "\n". Nonempty stream ends with "\n"
    :param output_stream: output stream. Contains byte-strings separated by "\n". Nonempty stream ends with "\n"
    :return: None
    """
    arr = []
    ind = 0
    for stream in input_streams:
        line = stream.readline()
        if len(line) > 0:
            arr.append([int(line), ind])
        ind += 1

    if len(arr) == 0:
        output_stream.write(bytes('\n', "utf8"))
        return

    heapq.heapify(arr)
    while len(arr) > 0:
        now = heapq.heappop(arr)
        output_stream.write(bytes(f"{now[0]}\n", "utf8"))
        next_line = input_streams[now[1]].readline()
        if len(next_line) > 0:
            heapq.heappush(arr, [int(next_line), now[1]])
    return

```
