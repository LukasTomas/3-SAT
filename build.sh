cd probsat; make; cd ..
ln -s  ./probsat/probsat ./algorithms/probsat
python3 run_experiment.py algorithms/probsat