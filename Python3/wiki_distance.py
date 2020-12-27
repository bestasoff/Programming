import typing as tp
import requests

from bs4 import BeautifulSoup
from pathlib import Path
from collections import defaultdict


# Directory to save your .json files to
# NB: create this directory if it doesn't exist
SAVED_JSON_DIR = Path(__file__).parent / 'visited_paths'


def distance(source_url: str, target_url: str, visited: tp.Any = None,
             dist: int = 0) -> tp.Optional[int]:
    """Amount of wiki articles which should be visited to reach the target one
    starting from the source url. Assuming that the next article is choosing
    always as the very first link from the first article paragraph (tag <p>).
    If the article does not have any paragraph tags or any links in the first
    paragraph then the target is considered unreachable and None is returned.
    If the next link is pointing to the already visited article, it should be
    discarded in favor of the second link from this paragraph. And so on
    until the first not visited link will be found or no links left in paragraph.
    NB. The distance between neighbour articles (one is pointing out to the other)
    assumed to be equal to 1.
    :param source_url: the url of source article from wiki
    :param target_url: the url of target article from wiki
    :param dist: distance between urls
    :param visited assistant util
    :return: the distance calculated as described above
    """
    if visited is None:
        visited = defaultdict(int)
    html = requests.get(source_url).text
    soup = BeautifulSoup(html, features='lxml')
    links = soup.find('div', attrs={'class': 'mw-parser-output'}).findAll("p", recursive=False)[0].findAll("a")
    if source_url not in visited:
        visited[source_url] = dist
        if source_url == target_url:
            return visited[source_url]
        for link in links:
            if 'href' not in link.attrs:
                continue
            if '#' in link['href']:
                continue
            if '.' in link['href']:
                continue
            if '/wiki/' not in link['href']:
                continue
            new_url = "https://ru.wikipedia.org" + link['href'] if '/wiki/' in link['href'] else link['href']
            if new_url in visited:
                continue
            distance(new_url, target_url, visited, dist + 1)
            break
    return visited[target_url] if target_url in visited else None
