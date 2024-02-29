# Sysex configurator

WebMIDI exploration


```shell 
docker build -t sysex-configurator .
docker run --rm --name sysex-configurator -p 5173:5173 -it -v $PWD:/app sysex-configurator 
```