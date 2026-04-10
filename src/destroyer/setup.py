import os
from glob import glob

data_files=[
    ('share/ament_index/resource_index/packages',
        ['resource/destroyer']),
    ('share/destroyer', ['package.xml']),
    (os.path.join('share', 'destroyer', 'launch'), glob('launch/*.launch.py')),
],
