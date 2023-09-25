import requests
import os
import numpy as np
from typing import List

from ..globals import DIR_DATA_CP4IM, CP4IM_DATASET_NAMES, CP4IM_DATASET_URL


def download_and_install(datasets: List[str] = None):
    if datasets is None:
        datasets = CP4IM_DATASET_NAMES

    for dataset in datasets:
        assert(dataset in CP4IM_DATASET_NAMES)
        print(f"Downloading CP4IM {dataset} dataset...")
        r = requests.get(CP4IM_DATASET_URL.format(dataset=dataset))
        lines = r.text.split('\n')
        num_features = 0
        data_line = None
        for i, line in enumerate(lines):
            if line.startswith('@data'):
                data_line = i + 1
            elif line.startswith('@') \
                and not line.startswith('@relation') \
                and not line.startswith('@class'):
                line_tag = line[1:line.find(':')]
                num_features = max(num_features, int(line_tag) + 1)

        assert(num_features > 0)
        assert(data_line is not None)

        feats = []
        labels = []
        for line in lines[data_line:]:
            if not line:
                continue
            sample = line.split(' ')
            items = map(int, sample[:-1])
            label = int(sample[-1])
            feat = np.zeros(num_features, dtype=np.int64)
            for i in items:
                feat[i] = 1
            feats.append(feat)
            labels.append(label)

        X = np.row_stack(feats)
        y = np.array(labels)

        print(f'# Features: {X.shape[1]}')
        print(f'# Samples: {X.shape[0]}')

        if not os.path.exists(DIR_DATA_CP4IM):
            os.makedirs(DIR_DATA_CP4IM)

        data_path = os.path.join(DIR_DATA_CP4IM, f'{dataset}.txt')
        with open(data_path, 'w') as fp:
            A = np.column_stack((X, y))
            np.savetxt(fp, A, fmt='%d')
