NetSurf bugs causing Acid2 brokenness
=====================================

Line(s):	Description:

1		No position: fixed support
		No {min,max}-width support
2		CSS engine incorrectly handles escape sequences
3		None: this line renders correctly
4-5		Plot order doesn't conform to appendix E.
		NetSurf plots in the order a,c,b instead of c,b,a
6		None: this line renders correctly
7-8		No generated content / :before or :after support
		(No :hover support, either, though that doesn't impact 
		the rendering of these lines)
9		None: this line renders correctly
10-11		Rounding errors in border plotting (nsgtk+cairo only)
12		No background-position: fixed support
13		CSS parser bugs:
			1: .parser { m\argin: 2em; }; .parser { height: 3em; }
			   means the second selector is "; .parser", which 
			   should be ignored. This results in the block 
			   height increasing to 3em.
14		None: this line renders correctly
(15)		No useful vertical-align support (tests line-height 
		calculations in table cells, but relies on default 
		vertical-align to shift image down and into the overflow area, 
		which is hidden)

