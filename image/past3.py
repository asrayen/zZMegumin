from PIL import Image
import os


def merge_image_regions(image_paths,
                        region,
                        save_path,
                        direction='horizontal'):
    """
    将多张图片的指定区域按指定方向拼接并保存
    
    :param image_paths: 图片绝对路径列表
    :param region: 裁剪区域元组 (left, upper, right, lower)
    :param save_path: 结果保存路径
    :param direction: 拼接方向 ('horizontal' 或 'vertical')
    """

    cropped_images = []
    for path in image_paths:
        try:
            with Image.open(path) as img:
                cropped = img.crop(region)
                cropped_images.append(cropped)
        except Exception as e:
            print(f"无法处理图片 {path}: {str(e)}")
            return

    if not cropped_images:
        print("无有效图片可处理")
        return

    # 创建结果图片尺寸
    if direction == 'horizontal':
        total_width = sum(img.width for img in cropped_images)
        max_height = max(img.height for img in cropped_images)
        new_img = Image.new('RGB', (total_width, max_height))
        x_offset = 0
        for img in cropped_images:
            new_img.paste(img, (x_offset, 0))
            x_offset += img.width
    else:
        total_height = sum(img.height for img in cropped_images)
        max_width = max(img.width for img in cropped_images)
        new_img = Image.new('RGB', (max_width, total_height))
        y_offset = 0
        for img in cropped_images:
            new_img.paste(img, (0, y_offset))
            y_offset += img.height

    # 确保保存目录存在
    save_dir = os.path.dirname(save_path)
    if save_dir:
        os.makedirs(save_dir, exist_ok=True)

    new_img.save(save_path)
    print(f"图片已保存至 {save_path}")


# 使用示例
if __name__ == "__main__":
    # 参数配置
    image_paths = [
        "C:\\Users\\15879\\OneDrive\\Desktop\\548E7BE47C56C22A3955B6781BDF1904.png",
        "C:\\Users\\15879\\OneDrive\\Desktop\\BB3B80A298E5A392F6037D5E984C1969.png",
        "C:\\Users\\15879\\OneDrive\\Desktop\\15F7288932E6D5F2CF768A2661DB6465.png"
    ]
    percent = [0, 0.7, 0.7]
    # 计算裁剪区域
    region = []  # (比例)
    for image_path in image_paths:
        with Image.open(image_path) as img:
            width, height = img.size
            left = 0
            upper = 0
            right = width
            lower = height * percent[]
            region.append((left, upper, right, lower))
    save_path = "E:\\verity\\image\\merged.jpg"
    direction = "vertical"  # 或 "horizontal"

    merge_image_regions(image_paths, region, save_path, direction)
