#!/bin/bash

coverage3.3 erase
# coverage3.3 run --source=.,dijkstra test_dijkstra.py && coverage3.3 report
coverage3.3 run --source=.,test_dijkstra,dijkstra -m test_dijkstra.test_dijkstra && coverage3.3 report

