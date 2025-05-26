from wordcloud import WordCloud
import jieba

w = WordCloud(width=1000,
              height=1000,
              font_path='C:\\Windows\\Fonts\\simkai.ttf',
              background_color='white')
f = open('词云(猫眼).txt', 'r', encoding='utf-8')
wordline = f.read()
w.generate(''.join(jieba.lcut(wordline)))
w.to_file('词云.png')
