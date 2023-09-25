from experiments.dataloader.cp4im import download_and_install as download_and_install_cp4im_data
from experiments.dataloader.synth import generate_synthetic_tree_data


def main():
    download_and_install_cp4im_data()
    generate_synthetic_tree_data()


if __name__ == '__main__':
    main()
