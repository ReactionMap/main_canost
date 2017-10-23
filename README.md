Convert a MDL Molfile to a CANOST code.


# Environment

## Development Environment

* gcc 4.0.1 (Mac OS X)
    
## Test Environment
* gcc 4.0.1 (Mac OS X)
* gcc 3.3.6 (Vine Linux)

# Compile

1. Extract main_canost-2.92.tar.gz 

```
% gzip -cd main_canost-2.91.tar.gz | tar xvf -
```

2. Change the directory 

```
% cd main_canost-2.91

```

3. Edit "Makefile" according to your system

4. Run 'make'

```
% make
```

Then you obtain 'main_canost' (or 'main_canost.exe' if you use Cygwin)

# Usage

```
% ./main_canost < output-format > < coding_level > [input-molfile-name] [output-filename]
```

## About output-format 

```
    -c     Output (not necessarily canonical) CANOST codes 
    	   for each atom in the input molecule

    -n     Output the sequences of serial numbers corresponding 
    	   to CANOST codes by -c option

    -p     Output CANOST codes and filiation

    -s     Output CANOST codes and symmetric substructures

    -d     Output CANOST codes and atoms whose orders depend on their conformation
   
    -u     Output the canonical CANOST code of the input molecule,
    	   i.e., the lexicographically leading CANOST codes by -c option,
	   and its corresponding sequence of serial numbers.

    -e     Output the canonical CANOST codes for each atom in the input molecule
    	   and thier corresponding sequences of serial numbers.
```

## level of substructure

```
     f     output CANOST codes for whole structures

     i     output CANOST codes for substructures
     	   consisting of atoms in "i" bonds from
	   a start atom.
           ( "i" must be one of 3, 4, 5, 6, 7, 8, 9, 10)
```

## Format of input files

```
MDL Molfile
```

# Sample data
* 20-Hydroxyecdysone.mol
* cubic.mol
