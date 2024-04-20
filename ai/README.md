## Development
First, make sure you have installed python3 virtual environment (and python3).  
Then, run the following commands to setup the virtual environment:  
**Linux**  
```bash
cp .env.example .env
python3 -m venv .venv
source .venv/bin/activate
which python
pip install -r requirements.txt
```
**Windows**  
```bash
copy .env.example .env
py -m venv .venv
.venv\Scripts\activate
where python
py -m pip install -r requirements.txt
```

To deactivate in both cases run:  
```bash
deactivate
```
## Train  
Ensure you've got downloaded a dataset and set valid path in .env file.  
Then run:    
**Linux**  
```bash
python3 train.py
```
**Windows**  
```bash
py train.py
```
