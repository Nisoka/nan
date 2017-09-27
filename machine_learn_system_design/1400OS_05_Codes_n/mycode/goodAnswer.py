# -*- coding: utf-8 -*-

import re
from sklearn.corss_validation import KFold
from sklearn.linear_model import LogisticRegression

code_match = re.compile('<pre>(.*?)</pre>',
                        re.MULTILINE|re.DOTAL)
link_match = re.compile('<a href="http://.*?".*?>(.*?)</a>',
                        re.MULTILINE|re.DOTAL)

def fetch_posts():
    for line in open("data.tsv", "r"):
        post_id, text = line.split("\t")
        yield int(post_id）, text.strip()

def extract_features_from_body(s):
    link_count_in_code = 0
    for match_str in code_match.findall(s):
        link_count_in_code +=
        len(link_match.findall(match_str))
    return len(link_match.findall(s)) - link_count_in_code


    

X = np.asarray([extract_features_from_body(text) for post_id,
                text in fetch_posts() if post_id in all_answers])
Y = []
knn = neighbors.KNeighborsClassifier()
knn.fit(X, Y)


scores = []
cv = KFold(n = len(x), k=10, indices = True)
for train, test in cv:
    #数组索引
    x_train, y_train = X[train], Y[train]
    x_test, y_test = X[test], Y[test]
    clf = neighbors.KNeighborsClassifier()
    clf.fit(X, Y)
    socres.append(clf.score(x_test, y_test))
print("Mean(scores)=%.5f\tStddev(scores)=%.5f"%(np.mean(scores, np.std(scores))))

#Mean(scores)=0.49100  Stddev(scores)=0.0288
#结果很差 49% 还不如抛硬币.

clf = LogisticRegression()
print(clf)
clf.fit(X, y)
print(np.exp(clf.intercept_), np.exp(clf.coef_.ravel()))
