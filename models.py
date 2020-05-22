import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import os
import datetime
import pdb
import time
import torchvision.models as torchmodels

class BaseModel(nn.Module):
    def __init__(self):
        super(BaseModel, self).__init__()
        if not os.path.exists('logs'):
            os.makedirs('logs')
        ts = time.time()
        st = datetime.datetime.fromtimestamp(ts).strftime('%Y-%m-%d_%H:%M:%S_log.txt')
        self.logFile = open('logs/' + st, 'w')

    def log(self, str):
        print(str)
        self.logFile.write(str + '\n')

    def criterion(self):
        # return nn.MSELoss()
        return nn.CrossEntropyLoss()

    def optimizer(self):
        return optim.SGD(self.parameters(), lr=0.001)

    def adjust_learning_rate(self, optimizer, epoch, args):
        lr = args.lr  # TODO: Implement decreasing learning rate's rules

        # if ((epoch + 1) % 50 == 0):
        #     lr *= 0.1
        
        for param_group in optimizer.param_groups:
            param_group['lr'] = lr
       


class LazyNet(BaseModel):
    def __init__(self):
        super(LazyNet, self).__init__()
        # TODO: Define model here
        self.fc1 = nn.Linear(3072, 10)


    def forward(self, x):
        # TODO: Implement forward pass for LazyNet
        # print(x.size());
        x = x.view(-1, 3072)
        x = self.fc1(x)
        return x
        

class BoringNet(BaseModel):
    def __init__(self):
        super(BoringNet, self).__init__()
        # TODO: Define model here
        self.fc1 = nn.Linear(32 * 32 * 3, 120)
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, 10)

    def forward(self, x):
        # TODO: Implement forward pass for BoringNet
        x = x.view(-1, 32 * 32 * 3)
        x = self.fc1(x)
        x = self.fc2(x)
        x = self.fc3(x)
        return x


class CoolNet(BaseModel):
    def __init__(self):
        super(CoolNet, self).__init__()
        # TODO: Define model here

        self.conv1 = nn.Conv2d(3, 10, kernel_size=5)

        self.conv2 = nn.Conv2d(10, 20, kernel_size=5)
    
        self.conv2_drop = nn.Dropout2d()
    
        self.fc1 = nn.Linear(500, 50)
    
        self.fc2 = nn.Linear(50, 10)
    

    def forward(self, x):
        # TODO: Implement forward pass for CoolNet
    
        x = F.relu(F.max_pool2d(self.conv1(x), 2))
    
        x = F.relu(F.max_pool2d(self.conv2_drop(self.conv2(x)), 2))
    
        x = x.view(-1, 500)
    
        x = F.relu(self.fc1(x))
    
        x = F.dropout(x, training=self.training)
    
        x = self.fc2(x)
    
        x = F.log_softmax(x, dim=1)
    
        return x
