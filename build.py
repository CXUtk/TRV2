import sys
import os
import shutil

# 把资源文件夹里的资源按照是否相等复制一份到输出目录


def copyFiles(resourcePath: str, dirName: str, outputPath: str):
    fileList = []
    totalFiles = 0
    for root, dirs, files in os.walk(resourcePath):
        for file in files:
            filePath = os.path.join(root, file)
            targetPath = os.path.join(
                outputPath, filePath.replace(resourcePath, dirName))
            totalFiles = totalFiles + 1

            if not os.path.exists(targetPath):
                fileList.append((filePath, targetPath))
                continue
            # 如果文件与目标文件不同，也要复制过去
            with open(filePath, 'rb') as fsrc:
                with open(targetPath, 'rb') as ftarget:
                    if fsrc.read() != ftarget.read():
                        fileList.append((filePath, targetPath))

    for src, dest in fileList:
        os.makedirs(os.path.dirname(dest), exist_ok=True)
        shutil.copy(src, dest)
        print('Copying {}'.format(dest))

    print('{} out of {} files are copied to output directory'.format(
        len(fileList), totalFiles))
    return fileList


def main():
    # 检查输入的路径是否合法
    if len(sys.argv) != 3:
        print('Usage: ./build.py <Resources_Dir> <Output_Dir>')
        return

    print('Parameter List: [' + ', '.join(sys.argv) + ']')

    resourcePath, outputPath = sys.argv[1], sys.argv[2]
    resDirectoryName = resourcePath[str.rfind(resourcePath, '/') + 1:]

    if not os.path.exists(resourcePath):
        print('Cannot find resource: {}'.format(resourcePath))
        return

    if not os.path.exists(outputPath):
        print('Cannot find output directory: {}'.format(outputPath))
        return

    # 绝对路径转相对路径
    resourcePath, outputPath = os.path.relpath(
        resourcePath), os.path.relpath(outputPath)

    fileList = copyFiles(
        resourcePath, resDirectoryName, outputPath)


if __name__ == '__main__':
    print('\n************************************* Executing Resources-copy Script *************************************\n')
    main()
    print('\n************************************* Finished Resources-copy Script **************************************\n')
