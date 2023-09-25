import experiments.dataloader.cp4im
import experiments.dataloader.synth


def main():
    experiments.dataloader.cp4im.download_and_install()
    experiments.dataloader.synth.generate_synthetic_tree_data()


if __name__ == '__main__':
    main()
