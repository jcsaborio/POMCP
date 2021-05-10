# POMCP 1.0

This is a reconstruction of the original POMCP source code used in the NIPS 2010 paper

"Online Monte-Carlo Planning in Large POMDPs"
by David Silver and Joel Veness, 2010

The original tarball is as of May 2021 no longer available.

This version has a few minor changes:
- The Cellar domain from:

	"Planning Under Uncertainty Through Goal-Driven Action Selection"
	by Juan Carlos Saborio and Joachim Hertzberg, 2019

- Small additions to support the Cellar domain:
    - GeneratePGS rollout policy in Simulator .h/.cpp
    - Command line options in main.cpp
