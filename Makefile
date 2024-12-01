CXX       := /usr/local/gcc/gcc-12.2.0/bin/g++
G++BINDIR := $(shell dirname $(shell readlink -f $(shell which $(CXX))))
HEADER    := $(G++BINDIR)/../include/c++/$(shell $(CXX) -dumpversion)/$(shell $(CXX) -dumpmachine)/bits/stdc++.h
PCH       := bits/stdc++.h.gch
SRC       := source.cpp
TARGET    := compro.out
CXXFLAGS  := -std=gnu++2b -O2 -Wall -Wextra -mtune=native -march=native -fconstexpr-depth=2147483647 -fconstexpr-loop-limit=2147483647 -fconstexpr-ops-limit=2147483647

.PHONY: test
test: $(TARGET)
	#echo "$$(./$<)"
	./$<

$(TARGET): $(SRC) $(PCH)
	$(CXX) $< $(CXXFLAGS) -o $@

$(PCH): $(HEADER)
	mkdir --parents $(@D)
	$(CXX) $< $(CXXFLAGS) -o $@

.PHONY: check
check: $(SRC) $(PCH)
	$(CXX) $< $(CXXFLAGS) -fsyntax-only

.PHONY: clean
clean:
	$(RM) $(TARGET)

.PHONY: cleanall
cleanall: clean
	$(RM) --recursive bits/
	$(RM) login.cookie.json
	$(RM) --recursive cases/
