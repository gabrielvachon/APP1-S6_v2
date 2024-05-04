from folder_streamer import FolderStreamer
import csv

INPUT_FOLDER = "data"
INPUT_FILE_EXTENSION = "svg"

OUTPUT_FILE_EXTENSTION = "png"
OUTPUT_FILE_PREFIX = "out-"
OUTPUT_RESOLUTION = 480
OUTPUT_CSV_FILE = "output.csv"


def generate_csv_file(
    input_folder,
    input_file_extension,
    output_file_extension,
    output_file_prefix,
    output_resolution,
    output_csv_file,
):
    files = FolderStreamer(input_folder, input_file_extension).stream()
    with open(output_csv_file, "w", newline="") as csvfile:
        csv_writer = csv.writer(csvfile, delimiter=";")
        for file in files:
            input_file = f"{input_folder}/{file}"
            file = file[: -len(input_file_extension) - 1]
            output_file = f"{output_file_prefix}{output_resolution}-{file}.{output_file_extension}"
            resolution = output_resolution
            csv_writer.writerow((input_file, output_file, resolution))


if __name__ == "__main__":
    generate_csv_file(
        input_folder=INPUT_FOLDER,
        input_file_extension=INPUT_FILE_EXTENSION,
        output_file_extension=OUTPUT_FILE_EXTENSTION,
        output_file_prefix=OUTPUT_FILE_PREFIX,
        output_resolution=OUTPUT_RESOLUTION,
        output_csv_file=OUTPUT_CSV_FILE,
    )
