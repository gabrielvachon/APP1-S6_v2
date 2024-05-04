import os
from typing import Sequence


class FolderStreamer:
    def __init__(self, folder_path, extensions=("jpg", "png", "svg")):
        self._folder_path = folder_path
        self._extensions = extensions

    def stream(self) -> Sequence[str]:
        files = tuple(
            file
            for file in os.listdir(self._folder_path)
            if file.endswith(self._extensions)
        )
        return tuple(file_name for file_name in files)

    @property
    def folder_path(self):
        return self._folder_path

    @folder_path.setter
    def folder_path(self, folder_path):
        self._folder_path = folder_path
