# By Gustin Julien & Sebati Ilias
## Slimming project

### How to use :
* Get into src/ repectory
* Type make
* Then
```sh
./slimming "input-file" "output-file" nbOfPixel-to-reduce
```

# Example of using

```sh
./slimming ../pnm/07.pnm ../pnmOutput/test07.pnm 500
```
### READ Rapport_Slimming.pdf

<p align="center">
  <img src="https://github.com/Julien-Gustin/Slimming/blob/master/slimming.png?raw=true" />
  <figcaption>(a) is the original image, (b) crushed's image (naive), (c) crushed's image with slimming</figcaption>
</p>

figure {
    display: inline-block;
    border: 1px dotted gray;
    margin: 20px; /* adjust as needed */
}
figure img {
    vertical-align: top;
}
figure figcaption {
    border: 1px dotted blue;
    text-align: center;
}
