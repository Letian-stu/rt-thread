import os
import shutil
import argparse
from jinja2 import Environment, FileSystemLoader
from openpyxl import load_workbook

# python.exe .\generate_fdb.py .\fdb.xlsx

output_dir = 'out'
src_dir = '../src'
inc_dir = '../inc'

env = Environment(
    loader=FileSystemLoader('templates'),
    trim_blocks=True,
    lstrip_blocks=True
)

parser = argparse.ArgumentParser(description="生成 FlashDB 和 RamDB 代码")
parser.add_argument('excel_file', help="Excel 文件路径，例如 fdb.xlsx")
args = parser.parse_args()

excel_file_path = args.excel_file

if not os.path.exists(excel_file_path):
    print(f"错误: 文件 '{excel_file_path}' 不存在！")
    exit(1)

wb = load_workbook(excel_file_path, data_only=True)

# 定义工作表配置
sheet_config = {
    "fdb": {"template_prefix": "flashdb_table", "output_prefix": "flashdb_table"},
    "rdb": {"template_prefix": "ramdb_table", "output_prefix": "ramdb_table"}
}

for sheet_name in ["fdb", "rdb"]:
    if sheet_name not in wb.sheetnames:
        print(f"警告: 工作表 '{sheet_name}' 不存在，跳过")
        continue

    ws = wb[sheet_name]
    config = sheet_config[sheet_name]
    variables = []

    for row in ws.iter_rows(min_row=2, values_only=True):
        try:
            if len(row) != 4:
                print(f"警告: 在工作表 '{sheet_name}' 中跳过无效行 {row}")
                continue
            
            var_name, var_type, array_size, macro_name = row
            variables.append({
                'var_name': var_name,
                'type': var_type,
                'array_size': array_size,
                'macro_name': macro_name
            })
        except Exception as e:
            print(f"错误: 在工作表 '{sheet_name}' 中解析行 {row} 失败. 错误信息: {e}")

    # 渲染模板
    header_template = env.get_template(f"{config['template_prefix']}.h.j2")
    header_content = header_template.render(variables=variables)
    source_template = env.get_template(f"{config['template_prefix']}.c.j2")
    source_content = source_template.render(variables=variables)

    # 生成输出路径
    header_file = f"{config['output_prefix']}.h"
    source_file = f"{config['output_prefix']}.c"
    header_path = os.path.join(output_dir, header_file)
    source_path = os.path.join(output_dir, source_file)

    # 保存文件
    with open(header_path, 'w') as f:
        f.write(header_content)
    with open(source_path, 'w') as f:
        f.write(source_content)

    # 复制到目标目录
    shutil.copy(header_path, os.path.join(inc_dir, header_file))
    shutil.copy(source_path, os.path.join(src_dir, source_file))

    print(f"Generated configuration file at {inc_dir} {src_dir}")
