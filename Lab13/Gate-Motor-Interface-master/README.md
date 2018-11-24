# Gate Motor Interface

Python interface to a gate motor controller using a microcontroller (uC). Tested ok with Python 2.7 and 3.5, under Ubuntu 16.04 and Windows 10.

## Run

Run program with:
```
cd ./src
python main.py
```



## Dependencies

- **wxPython:** `pip install wxPython`
- **pyserial:** `pip install pyserial`

*Linux Note:* run `pip`/`pip3` with `sudo`. To install wxPython, first install the dependencies listed in:  https://github.com/wxWidgets/Phoenix#prerequisites . Please note that the pip installation could take a long time.

*Linux Note:* running pyserial code could need `sudo` or changing user groups as below:

```sudo usermod -a -G dialout $USER```



## Instructions


- Modify communication functions in `src/uC_interface.py` to adjust to your own communication protocol while mantaining consistency with the states defined in `src/gate.py`, shown in the following state machine diagram (those states are interpreted in the GUI code in `src/main.py` to generate the gate animation).



