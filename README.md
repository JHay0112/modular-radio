# Modular Radio

This is a suite of programs that simulate components of a radio system. All
components take an input through stdin and return an output through stdout.
This permits the programs be modularly piped together to simulate a radio
system in a modular manner e.g.

```
./tx | ./channel | ./rx
```

Currently, the programs are built in C. However, any other language that
permits reading and writing of stdin and stdout would be a suitable substitute.
C was chosen as a challenge for myself to write multithreaded C programs
rather than as an appropriate choice for the application.

For anyone looking for radio simulators please see GNU Radio.