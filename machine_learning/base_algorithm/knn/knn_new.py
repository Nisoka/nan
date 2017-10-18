
from numpy import *
import operator

def classify(entry, data_x, data_y, k = 5):
    data_m = data_x.shape[0]
    
    diff_value = tile(entry, (data_m, 1)) - data_x
    print(diff_value)
    square_diff = diff_value**2
    square_diff_sum = square_diff.sum(axis = 1)
    distances = square_diff_sum**0.5
    
    sorted_distances_index = distances.argsort()
    
    class_dict = {}
    for i in range(k):
        tmp_label = data_y[sorted_distances_index[i]]
        if class_dict[tmp_label] not in class_dict.keys():
            class_dict[tmp_label] = 0
        class_dict[tmp_label] += 1

        
    sorted_class_dict = sorted(class_dict.items(), key=operator.itemgetter(1), reverse=True)
    print(sorted_class_dict)
    return sorted_class_dict[0][0]

        
class cKnn:
    def __init__(self, data_x, data_y):
        self.data_x = data_x
        self.data_y = data_y

    def train(self):
        return True

    
    def predict(self, entry, k):
        return classify(entry, self.data_x, self.data_y, k)

    def train_err(self):
        print("KNN don't need train")
        return True



def load_data(filename):
    data = []
    fr = open(filename)
    for line in fr.readlines():
        line_list = line.strip().split('\t')
        data.append(line_list)
    
    data = mat(data)
    n = data.shape[1]
    print(data)
    data_x = data[:, :-1]
    data_y = data[:, -1]
    #print(data_x)
    #print(data_y)
    return data_x, data_y


def auto_norm(data_x):
    features_min = data_x.min(0)
    features_max = data_x.max(0)
    ranges = features_max - features_min
    
    m = data_x.shape[0]
    data_norm = data_x - tile(features_min, (m, 1))
    data_norm = data_norm / tile(features_max, (m, 1))
    return data_norm, ranges, features_min

def func_data_test():
    train_ratio = 0.7
    data_x, data_y = load_data('datingTestSet2.txt')
    data_x = data_x.astype(float)
    data_x, ranges, features_min = auto_norm(data_x)
    
    m = data_x.shape[0]
    train_start =int(m*(1-0.7))
    err_cnt = 0

    knn = cKnn(data_x[train_start:m], data_y[train_start:m])
    
    for i in range(train_start):
        ret = knn.predict(data_x[i], 7)
        if ret != data_y[i]:
            err_cnt += 1
            
            
    print("Test Error Count is %d, test cnt is %d " %(err_cnt, train_start))
    
    

if __name__ == '__main__':
    func_data_test()
        







    
