
import sys

def main(argc, argv):
    if argc < 2:
        print(f"usage: {argv[0]} \"-- --- .-. ... ./.... . .-. .\"")
        return

    morse_string = argv[1]
    morse_to_bits = []

    for i in morse_string:
        if i == ".":
            morse_to_bits.append(1)
            morse_to_bits.append(0)
        if i == "-":
            morse_to_bits.append(1)
            morse_to_bits.append(1)
            morse_to_bits.append(1)
            morse_to_bits.append(0)
        if i == " ":
            # space is 3 units but each char has a space at the end of it
            morse_to_bits.append(0)
            morse_to_bits.append(0)
        if i == "/":
            # space between words is 7 units, but each char has a space after it
            morse_to_bits.append(0)
            morse_to_bits.append(0)
            morse_to_bits.append(0)
            morse_to_bits.append(0)
            morse_to_bits.append(0)
            morse_to_bits.append(0)

    print("uint8_t morse_converted[] = {", end="")
    for i in morse_to_bits:
        print(f"{i}, ", end="")
    print("};")

if __name__ == "__main__":
    main(len(sys.argv), sys.argv)