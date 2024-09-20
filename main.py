#!/usr/bin/env python

import ctypes
import numpy as np
import pathlib

def doShortestPath(cppLibrary):
  seed = 0x533D
  np.random.seed(seed)

  height = np.int32(10)
  width = np.int32(10)
  grid = np.random.choice(np.array([0,1], dtype=np.int32), size=(height, width), p=[0.9,0.1])
  print(f'Built grid:\n{grid}')

  start = np.array([0, 0], dtype=np.int32)
  goal = np.array([height-1, width-1], dtype=np.int32)

  print(f'Want to find shortest path from {start} to {goal}')

  length = cppLibrary.shortestPathLength(start, goal, grid, height, width)

  print(f'The shortest path has length {length}')

if __name__ == "__main__":
  # Load the shared library into ctypes
  c_libname = pathlib.Path().absolute() / "libcmult.so"
  c_lib = ctypes.CDLL(c_libname)
  c_lib.cmult.restype = ctypes.c_float

  cpp_libname = pathlib.Path().absolute() / "libcppmult.so"
  cppLibrary = ctypes.CDLL(cpp_libname)
  cppLibrary.cppMult.restype = ctypes.c_float

  cppLibrary.shortestPathLength.restype = ctypes.c_int32
  cppLibrary.shortestPathLength.argtypes = [
    np.ctypeslib.ndpointer(ctypes.c_int32, flags="C_CONTIGUOUS"),
    np.ctypeslib.ndpointer(ctypes.c_int32, flags="C_CONTIGUOUS"),
    np.ctypeslib.ndpointer(ctypes.c_int32, flags="C_CONTIGUOUS"),
    ctypes.c_int32,
    ctypes.c_int32
  ]

  # x, y = 6, 2.3
  # answer = c_lib.cmult(x, ctypes.c_float(y))
  # print(f"    In Python c-result: int: {x} float {y:.1f} return val {answer:.1f}")
  # answer = cppLibrary.cppMult(x, ctypes.c_float(y))
  # print(f"    In Python cpp-result: int: {x} float {y:.1f} return val {answer:.1f}")

  doShortestPath(cppLibrary)