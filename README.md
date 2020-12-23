# RISCV

```shell script
find . -name "uvm_misc.cpp" | xargs -I{} sed -ie  '272s/.*/  std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){return std::toupper(c);});/' {}
find . -name SCV -type d |xargs -I{} find {} -name "configure" | xargs -I{} sed -ie '/scv_cv_sc_version="unknown"/ s/.*/         scv_cv_sc_version="2003003"/' {}
```
