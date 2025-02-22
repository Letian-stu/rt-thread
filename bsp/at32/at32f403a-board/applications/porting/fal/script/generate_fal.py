from openpyxl import load_workbook
from jinja2 import Environment, FileSystemLoader

# 自定义过滤器：rjust
def rjust_filter(value, width):
    return str(value).rjust(width)

def read_partition_table(file_path):
    """读取Excel中的分区表并打印出来"""
    wb = load_workbook(file_path, data_only=True)
    ws = wb.active

    # 存储分区数据
    onchip_partitions = []
    nor_partitions = []
    ram_partitions = []  # 新增RAM分区列表

    for row in ws.iter_rows(min_row=2, values_only=True):
        try:
            size_kb = float(row[4]) if row[4] is not None else 0
            start_addr_decimal = row[2]
            start_addr_hex = row[3]

            partition = {
                'dev': row[0],
                'name': row[1],
                'start_addr': start_addr_decimal,
                'start_hex': start_addr_hex,
                'size_kb': size_kb,
                'size_bytes': int(size_kb*1024)
            }

            if row[0] == "OnChip":
                onchip_partitions.append(partition)
            elif row[0] == "w25q64":
                nor_partitions.append(partition)
            elif row[0] == "RAM":  # 新增RAM设备类型判断
                ram_partitions.append(partition)

        except (ValueError, TypeError) as e:
            print(f"Skipping invalid row: {row}. Error: {e}")

    return onchip_partitions, nor_partitions, ram_partitions

def generate_cfg_file(onchip_partitions, nor_partitions, ram_partitions, template_path, output_path):
    """Render the template and generate the fal_cfg.h file"""
    env = Environment(
        loader=FileSystemLoader(template_path),
        trim_blocks=True,
        lstrip_blocks=True
    )
    env.filters['rjust'] = rjust_filter

    template = env.get_template('fal_cfg_template.h.j2')

    # 获取设备名称
    onchip_device = onchip_partitions[0]['dev'] if onchip_partitions else 'onchip_flash'
    nor_device = nor_partitions[0]['dev'] if nor_partitions else 'w25q64'
    ram_device = ram_partitions[0]['dev'] if ram_partitions else 'ram'  # 新增RAM设备名称

    data = {
        'onchip_device_macro': onchip_device,
        'nor_device_macro': nor_device,
        'ram_device_macro': ram_device,  # 新增RAM设备宏
        'onchip_partitions': onchip_partitions,
        'nor_partitions': nor_partitions,
        'ram_partitions': ram_partitions  # 新增RAM分区数据
    }

    with open(output_path, 'w') as f:
        f.write(template.render(data))

    print(f"Generated configuration file at {output_path}")

# Usage
file_path = 'fal.xlsx'  # Path to the uploaded Excel file
template_path = 'templates'     # Directory where the template is stored
output_path = 'out/fal_cfg.h'  # Output file path

onchip_partitions, nor_partitions, ram_partitions = read_partition_table(file_path)
generate_cfg_file(onchip_partitions, nor_partitions, ram_partitions, template_path, output_path)
