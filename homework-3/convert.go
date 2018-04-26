package main

import (
	"fmt"
	"image"
	"image/color"
	_ "image/gif"
	_ "image/jpeg"
	"log"
	"os"
	"strconv"

	"github.com/nfnt/resize"
)

var OUT = "bump_map.pgm"

func main() {
	if len(os.Args) != 3 {
		log.Fatal("Usage: ./convert [height] [width]")
	}
	height, err := strconv.ParseInt(os.Args[1], 10, 64)
	if err != nil {
		log.Fatal(err)
	}

	width, err := strconv.ParseInt(os.Args[2], 10, 64)
	if err != nil {
		log.Fatal(err)
	}

	img, _, err := image.Decode(os.Stdin)
	if err != nil {
		log.Fatal(err)
	}

	img = resize.Resize(uint(width), uint(height), img, resize.Lanczos3)
	gray := image.NewGray(img.Bounds())

	out, err := os.Create(OUT)
	if err != nil {
		log.Fatal(err)
	}
	defer out.Close()

	fmt.Fprintf(out, "P2 %d %d 255\n", width, height)
	for row := 0; row < int(height); row++ {
		for col := 0; col < int(width); col++ {
			gray.Set(col, row, color.GrayModel.Convert(img.At(col, row)))
			fmt.Fprintf(out, "%d ", gray.GrayAt(col, row).Y)
		}
		fmt.Fprintf(out, "\n")
	}
}
