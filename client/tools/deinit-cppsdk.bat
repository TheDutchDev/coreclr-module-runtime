@echo off

if EXIST ../cpp-sdk/SDK.h (
  echo alt:V C# - Using external cpp-sdk, deiniting local cpp-sdk
  git submodule deinit -f ../cpp-sdk
)
