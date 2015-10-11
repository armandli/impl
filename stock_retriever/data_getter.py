#!/usr/bin/python2

import urllib
from datetime import date
from csv import DictReader
from argparse import ArgumentParser
from multiprocessing import Process

#                                                             SYM                   MON   DAY   YEAR
yahoo_addr = "http://real-chart.finance.yahoo.com/table.csv?s={0}&a=00&b=2&c=1962&d={1}&e={2}&f={3}&g=d&ignore=.csv"

def get_arguments():
    parser = ArgumentParser()
    parser.add_argument('--csv', help='csv file containing symbol name', type=str, required=True)
    parser.add_argument('--col', help='csv file column name', type=str, required=True)
    parser.add_argument('--delim', help='csv file delimiter', type=str, required=True)
    parser.add_argument('--prefix', help='output file directory and prefix', type=str, required=True)
    args = parser.parse_args()
    return args

def get_day(date):
    d = date.day - 1
    if d < 10:
        return "0" + str(d)
    else:
        return str(d)

def get_month(date):
    d = date.month - 1
    if d < 10:
        return "0" + str(d)
    else:
        return str(d)

def get_year(date):
    return date.year

def download_data(symbol, date, out_prefix):
    address = yahoo_addr.format(symbol, get_month(date), get_day(date), get_year(date))
    data = urllib.urlopen(address)
    outfile = out_prefix + "_" + str(date.month) + "." + str(date.day) + "." + str(date.year) + "_" + symbol
    with open(outfile, 'w') as output:
        output.write(data.read())

def get_pricing_history(csvfile, colname, delim, date, out_prefix):
    processes = []
    with open(csvfile) as fd:
        reader = DictReader(fd, delimiter=delim)
        for row in reader:
            symbol = row[colname]
            processes.append(Process(target=download_data, args=(symbol, date, out_prefix)))
    forking = 1000
    for i in xrange(0, len(processes), forking):
        for j in xrange(i, min(i + forking, len(processes))):
            processes[j].start()
        for j in xrange(i, min(i + forking, len(processes))):
            processes[j].join()

if __name__ == '__main__':
    args = get_arguments()
    today = date.today()
    get_pricing_history(args.csv, args.col, args.delim, today, args.prefix)
