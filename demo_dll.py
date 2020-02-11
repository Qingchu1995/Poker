# An example of using dll
from ctypes import *
mydll = cdll.LoadLibrary("poker64.dll")
a1 = c_bool(0) #false
a2 = c_bool(1) #true
mydll.play_2p(a1,a2) # play_2p(bool unequalF, bool printF)
mydll.play(a2) # play(bool printF)
