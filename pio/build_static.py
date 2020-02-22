import gzip
import zlib
import os
import htmlark

dataFolder = 'src/data/'
staticFolder = 'src/static/'

def toHeader(source, dest, fileName):
    destFile = os.path.join(dest, fileName)
    #print(source)
    packed_html = htmlark.convert_page(source, ignore_errors=True)
    with gzip.open(destFile, "wt") as f:
        f.write(packed_html)

    #zipped = zlib.compress(packed_html.encode('utf-8'))
    zipped = open(destFile, 'rb').read()
    #os.remove(destFile)
    #print(zipped)
    destFile = destFile + ".h"
    safeName = fileName.replace('.', '_')

    with open(destFile, "wt") as f:
        f.write('#define ' + safeName + '_len ' + str(len(zipped)) + '\n')
        f.write('const uint8_t ' + safeName + '[] PROGMEM = {')
        
        for counter, b in enumerate(zipped):
            if counter % 20 == 0:
                f.write("\n")
            f.write(hex(b))
            if counter < len(zipped)-1:
                f.write(",")

        f.write('\n};')
        f.close()

def buildStatic():
    toHeader("html/index.html", "src/static", "index.html.gz")

if __name__ == '__main__' or __name__ == "SCons.Script":
    print("building static")
    buildStatic()