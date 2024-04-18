## Development
First, make sure you have installed python3 virtual environment (and python3).  
Then, run the following commands to setup the virtual environment:  
**Linux**  
```bash
python3 -m venv .venv
source .venv/bin/activate
which python
pip install -r requirements.txt
```
**Windows**  
```bash
py -m venv .venv
.venv\Scripts\activate
where python
py -m pip install -r requirements.txt
```

To deactivate in both cases run:  
```bash
deactivate
```
## Run  
**Linux**  
```bash
python3 main.py
```
**Windows**  
```bash
py main.py
```
