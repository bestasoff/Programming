from collections import deque
import typing as tp


def normalize_path(path: str) -> str:
    """
    :param path: unix path to normalize
    :return: normalized path
    """
    q: tp.Deque[str] = deque()
    double_dot: int = 0
    dot: int = 0
    if not path:
        return '.'
    res: str = ""
    slash: bool = (path[0] == '/')
    npath = [el for el in path.split('/') if el != '']
    for direct in npath:
        if direct == '.' and len(npath) == 1:
            return '.'
        if direct == '.' and npath[0] == direct:
            dot += 1
            for el in npath[1:]:
                if el == '..':
                    if q and q[-1] != "..":
                        q.pop()
                    else:
                        q.append(el)
                elif el == '.':
                    continue
                else:
                    q.append(el)
            break
        if direct == '.':
            continue
        elif direct == '..':
            double_dot += 1
            if q:
                q.pop()
                double_dot += 1
            else:
                q.append('..')
        else:
            q.append(direct)
    # print(q)
    if q:
        if len(q) == 1 and double_dot != 0 and double_dot % 2 == 0:
            res += '/' + q.pop()
            return res

        if (double_dot != 0 and double_dot % 2 == 0) and (not dot) and slash and q[0] != '..':
            res += '/'

        while q:
            el = q.popleft()
            if (el == '..') and (double_dot != 0 and double_dot % 2 == 0) and (not dot):
                res += '/'
                continue
            if q:
                res += el + '/'
            else:
                res += el
    elif double_dot == 2:
        res += '.'
    else:
        res += '/'

    return res
