import jieba
import stylecloud
import pandas as pd
from PIL import Image
from collections import Counter
from pyecharts.charts import Geo
from pyecharts.charts import Bar
from pyecharts.charts import Line
from pyecharts.charts import Pie
from pyecharts.charts import Calendar
from pyecharts.charts import WordCloud
from pyecharts import options as opts
from pyecharts.commons.utils import JsCode
from pyecharts.globals import ThemeType, SymbolType, ChartType

df = pd.read_excel("星际穿越(时光).xlsx")
color_js = """new echarts.graphic.LinearGradient(0, 0, 1, 0,
    [{offset: 0, color: '#009ad6'}, {offset: 1, color: '#ed1941'}], false)"""

df_star = df.groupby('评分')['评论'].count()
df_star = df_star.sort_values(ascending=True)
x_data = [str(i) for i in list(df_star.index)]
y_data = df_star.values.tolist()
b1 = (Bar().add_xaxis(x_data).add_yaxis(
    '', y_data, itemstyle_opts=opts.ItemStyleOpts(
        color=JsCode(color_js))).reversal_axis().set_series_opts(
            label_opts=opts.LabelOpts(position='right')).set_global_opts(
                yaxis_opts=opts.AxisOpts(name='评分等级'),
                xaxis_opts=opts.AxisOpts(name='人/次'),
                title_opts=opts.TitleOpts(title='评分等级分布',
                                          pos_left='45%',
                                          pos_top="5%"),
                legend_opts=opts.LegendOpts(type_="scroll",
                                            pos_left="85%",
                                            pos_top="28%",
                                            orient="vertical")))

df_star = df.groupby('评分')['评论'].count()
x_data = [str(i) for i in list(df_star.index)]
y_data = df_star.values.tolist()
p1 = (Pie(init_opts=opts.InitOpts(width='800px', height='600px')).add(
    '',
    [list(z) for z in zip(x_data, y_data)],
    radius=['10%', '30%'],
    center=['65%', '60%'],
    label_opts=opts.LabelOpts(is_show=True),
).set_colors([
    "blue", "green", "#800000", "red", "#000000", "orange", "purple", "red",
    "#000000", "orange", "purple"
]).set_series_opts(label_opts=opts.LabelOpts(formatter='评分{b}: {c} \n ({d}%)'),
                   position="outside"))

b1.overlap(p1)
b1.render_notebook()
