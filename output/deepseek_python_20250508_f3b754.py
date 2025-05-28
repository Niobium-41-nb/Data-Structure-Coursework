import os
import chardet

def convert_csv_encoding_to_utf8(filename):
    # 读取文件二进制内容
    with open(filename, 'rb') as f:
        raw_data = f.read()
    
    # 检测文件编码
    result = chardet.detect(raw_data)
    original_encoding = result['encoding']
    confidence = result['confidence']
    
    print(f"处理文件: {filename}")
    print(f"检测编码: {original_encoding} (置信度: {confidence:.2%})")
    
    # 跳过已经是UTF-8编码的文件
    if original_encoding and original_encoding.lower() in ['utf-8', 'utf_8', 'utf8']:
        print("文件已经是UTF-8编码，无需转换\n")
        return
    
    try:
        # 尝试解码内容
        decoded_content = raw_data.decode(original_encoding if original_encoding else 'utf-8', 
                                       errors='replace')
    except (UnicodeDecodeError, LookupError) as e:
        print(f"解码失败: {str(e)}")
        print("尝试使用备用编码解码...")
        try:
            decoded_content = raw_data.decode('gbk', errors='replace')
        except Exception as e:
            print(f"备用解码失败: {str(e)}\n")
            return
    
    # 写入临时文件
    temp_filename = filename + '.tmp'
    with open(temp_filename, 'w', encoding='utf-8') as f:
        f.write(decoded_content)
    
    # 替换原始文件
    os.replace(temp_filename, filename)
    print("文件转换成功\n")

def main():
    # 获取当前目录下所有CSV文件
    csv_files = [f for f in os.listdir('.') 
                if os.path.isfile(f) and f.lower().endswith('.csv')]
    
    if not csv_files:
        print("当前目录未找到CSV文件")
        return
    
    print(f"找到 {len(csv_files)} 个CSV文件需要处理\n")
    for idx, csv_file in enumerate(csv_files, 1):
        print(f"正在处理文件 ({idx}/{len(csv_files)})")
        try:
            convert_csv_encoding_to_utf8(csv_file)
        except Exception as e:
            print(f"处理文件 {csv_file} 时发生错误: {str(e)}\n")

if __name__ == '__main__':
    try:
        import chardet
    except ImportError:
        print("请先安装 chardet 库: pip install chardet")
        exit(1)
    
    main()