import os
import sys
import codecs
from chardet.universaldetector import UniversalDetector

# 把文件夹中所有 *.cpp, *.h, *.hpp, *.inl, ... 等源码文件全部转换为utf8编码
__exts__ = (
    ".cpp", ".h", ".hpp", ".inl", ".frag", ".vert"
)

# 检测文件的编码


def detectEncoding(file):
    detector = UniversalDetector()
    with open(file, 'rb') as f1:
        detector.feed(f1.read())
    detector.close()
    return detector.result


def encodeToTarget(fileName, encoding, target):
    # TODO: This is dangerous ^^||, would need a backup option :)
    # NOTE: Use 'replace' option which tolerates errorneous characters
    data = codecs.open(fileName, 'rb', encoding, 'replace').read()
    open(fileName, 'wb').write(data.encode(target, 'replace'))


def main():
    # 检查输入的路径是否合法
    if len(sys.argv) != 3:
        print('Usage: ./encoding.py <Encoding_Dir> <Target Encoding>')
        return

    sourceDir = os.path.relpath(sys.argv[1])
    targetEncoding = sys.argv[2]

    for root, dirs, files in os.walk(sourceDir):
        for file in files:
            fileName = os.path.join(root, file)
            if fileName.endswith(__exts__):
                result = detectEncoding(fileName)

                if result['confidence'] == 0 and result['encoding'] == None:
                    print('Empty: {}'.format(fileName))
                    continue

                encoding = result['encoding'].lower()
                confidence = result['confidence']

                if confidence < 0.7:
                    print('Skipped: {}'.format(fileName))
                    continue

                if encoding != targetEncoding:
                    encodeToTarget(fileName, encoding, targetEncoding)
                    print('Encoding: {}'.format(fileName))
                else:
                    print('Skipped {}: {}'.format(targetEncoding, fileName))


if __name__ == '__main__':
    print('\n************************************* Executing Encoding-check Script *************************************\n')
    main()
    print('\n************************************* Finished Encoding-check Script **************************************\n')