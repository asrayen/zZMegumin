import jieba
import pandas as pd
from collections import defaultdict

stopwords = set()
fr = open(r'情感分析&爬虫\停用词.txt', 'r', encoding='utf-8')
for word in fr:
    stopwords.add(word.strip())
not_word_file = open(r'情感分析&爬虫\否定词.txt', 'r+', encoding='utf-8')
not_word_list = not_word_file.readlines()
not_word_list = [w.strip() for w in not_word_list]

degree_file = open(r'情感分析&爬虫\程度副词.txt', 'r+', encoding='utf-8')
degree_list = degree_file.readlines()
degree_list = [item.split(',')[0] for item in degree_list]

with open('stopwords.txt', 'w', encoding='utf-8') as f:
    for word in stopwords:
        if (word not in not_word_list) and (word not in degree_list):
            f.write(word + '\n')


def seg_word(sentence):
    seg_list = jieba.cut(sentence)
    seg_result = []
    for i in seg_list:
        seg_result.append(i)
    stopwords = set()
    with open('stopwords.txt', 'r', encoding='utf-8') as fr:
        for i in fr:
            stopwords.add(i.strip())
    return list(filter(lambda x: x not in stopwords, seg_result))


def classify_words(word_list):
    sen_file = open(r'情感分析&爬虫\BosonNLP_sentiment_score.txt',
                    'r+',
                    encoding='utf-8')
    sen_list = sen_file.readlines()
    sen_dict = defaultdict()
    for i in sen_list:
        if len(i.split(' ')) == 2:
            sen_dict[i.split(' ')[0]] = i.split(' ')[1]

    not_word_file = open(r'情感分析&爬虫\否定词.txt', 'r+', encoding='utf-8')
    not_word_list = not_word_file.readlines()

    degree_file = open(r'情感分析&爬虫\程度副词.txt', 'r+', encoding='utf-8')
    degree_list = degree_file.readlines()
    degree_dict = defaultdict()
    for i in degree_list:
        degree_dict[i.split(',')[0]] = i.split(',')[1]

    sen_word = dict()
    not_word = dict()
    degree_word = dict()
    for i in range(len(word_list)):
        word = word_list[i]
        if word in sen_dict.keys(
        ) and word not in not_word_list and word not in degree_dict.keys():
            sen_word[i] = sen_dict[word]
        elif word in not_word_list and word not in degree_dict.keys():
            not_word[i] = -1
        elif word in degree_dict.keys():
            degree_word[i] = degree_dict[word]

    sen_file.close()
    not_word_file.close()
    degree_file.close()

    return sen_word, not_word, degree_word


def score_sentiment(sen_word, not_word, degree_word, seg_result):

    W = 1
    score = 0

    sentiment_index = -1

    sentiment_index_list = list(sen_word.keys())

    for i in range(0, len(seg_result)):

        if i in sen_word.keys():

            score += W * float(sen_word[i])

            sentiment_index += 1
            if sentiment_index < len(sentiment_index_list) - 1:

                for j in range(sentiment_index_list[sentiment_index],
                               sentiment_index_list[sentiment_index + 1]):

                    if j in not_word.keys():
                        W *= -1
                    elif j in degree_word.keys():
                        W *= float(degree_word[j])

        if sentiment_index < len(sentiment_index_list) - 1:
            i = sentiment_index_list[sentiment_index + 1]
    return score


def sentiment_score(sentence):

    seg_list = seg_word(sentence)

    sen_word, not_word, degree_word = classify_words(seg_list)

    score = score_sentiment(sen_word, not_word, degree_word, seg_list)
    return score


def readExcel(path):
    df_dict = {}
    df = pd.read_excel(path)
    df.fillna("", inplace=True)
    df_list = []
    for i in df.index.values:
        df_line = df.loc[i, ['评论']].to_dict()
        df_list.append(df_line)
    df_dict['data'] = df_list
    return df_dict


path = '星际穿越(猫眼).xlsx'


def main():
    # comments = readExcel(path)
    # good = 0
    # bad = 0
    # cnt = 0
    # for comment in comments['data']:
    #     for elem in comment['评论']:
    #         # with open('词云(猫眼).txt', 'a', encoding='utf-8') as f:
    #         #     f.write(elem)
    #         cnt += 1
    #         if (sentiment_score(elem) > 0):
    #             good += sentiment_score(elem)
    #         elif (sentiment_score(elem) < 0):
    #             bad -= sentiment_score(elem)
    #         print('good = ', good, 'bad = ', bad, '次数 = ', cnt)
    cnt = 0
    good = 0
    bad = 0
    comment = open(r'C:\Users\15879\Desktop\shiguang_taitan(8000).txt',
                   'r',
                   encoding='utf-8')
    wordline = comment.readlines()
    for elem in wordline:
        cnt += 1
        if (sentiment_score(elem) > 0):
            good += sentiment_score(elem)
        elif (sentiment_score(elem) < 0):
            bad -= sentiment_score(elem)
        print('good = ', good, 'bad = ', bad, '次数 = ', cnt)


if __name__ == '__main__':
    main()
