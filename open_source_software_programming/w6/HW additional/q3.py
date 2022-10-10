import gym
import ale_py
import copy
from ale_py.roms import Breakout
from random import randrange
from ale_py import ALEInterface
from q1 import PolicyIteration


env = gym.make("Taxi-v3")
policy_iteration = PolicyIteration(env)
observation = env.reset()  # 매 에피소드 시작시 환경을 초기화한다.
env2 = copy.deepcopy(env)
observation2 = observation
discount_factor = 0.9
total_evaluation = 50
eval_batch = 1
for a in range(total_evaluation//eval_batch):
    policy_iteration.policy_evaluation(eval_batch, discount_factor)
    policy_iteration.policy_improvement()
done = False
for a in range(50):
    env.render()
    state = list(env.decode(observation))
    action = policy_iteration.get_action(state)  # Your agent code here
    observation, reward, done, info = env.step(action)
    if done:
        print("eval batch: ", eval_batch, "진행 횟수: ", a)
        if reward == 20:
            print("성공 여부: 성공")
        else:
            print("성공 여부: 실패")
        break
print("====================================")
policy_iteration.reset(env2)
observation2 = env2.s
eval_batch = 10
for a in range(total_evaluation//eval_batch):
    policy_iteration.policy_evaluation(eval_batch, discount_factor)
    policy_iteration.policy_improvement()
done = False
for a in range(50):
    env2.render()
    state = list(env2.decode(observation2))
    action = policy_iteration.get_action(state)
    observation2, reward, done, info = env2.step(action)
    if done:
        print("eval batch: ", eval_batch, "진행 횟수: ", a)
        if reward == 20:
            print("성공 여부: 성공")
        else:
            print("성공 여부: 실패")
        break
env.close()
env2.close()
