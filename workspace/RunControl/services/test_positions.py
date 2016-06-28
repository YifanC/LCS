__author__ = 'matthias'

import argparse
from services.positions import *

# Ask for the arguments
parser = argparse.ArgumentParser(description='Test script for the defined scans')

parser.add_argument("-c", "--configfile", action='store', dest='file', required=True,
                    help='configuration file to inspect', type=str)

arguments = parser.parse_args()

pos = Positions(-99)
pos.load(arguments.file)

pos.plotMovement()


