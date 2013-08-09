using System.Collections.Generic;
using Nitrogen.Content.UserGenerated.Halo4.Megalo;

// ReSharper disable once CheckNamespace
namespace ModeManager
{
    public static class Megalo
    {
        public static KeyValuePair<string, object> CreateCondition(string name, object data)
        {
            return new KeyValuePair<string, object>(name, data);
        }

        public static MegaloAction SetMegaloAction(string actionName, MegaloTrigger triggerToAddTo)
        {
            return SetMegaloAction(actionName, triggerToAddTo, new List<KeyValuePair<string, object>>());
        }
        public static MegaloAction SetMegaloAction(string actionName, MegaloTrigger triggerToAddTo, KeyValuePair<string, object> argument)
        {
            return SetMegaloAction(actionName, triggerToAddTo, new List<KeyValuePair<string, object>> { argument });
        }
        public static MegaloAction SetMegaloAction(string actionName, MegaloTrigger triggerToAddTo, List<KeyValuePair<string, object>> arguments)
        {
            var action = new MegaloAction(actionName);
            foreach (var argument in arguments)
                action.Arguments.Set(argument.Key, argument.Value);
            triggerToAddTo.Actions.Add(action);

            return action;
        }

        public static MegaloCondition SetMegaloCondition(string conditionName, MegaloTrigger triggerToAddTo, MegaloAction startAction, List<KeyValuePair<string, object>> arguments, int unionId = 0)
        {
            var condition = new MegaloCondition(conditionName);
            foreach (var argument in arguments)
                condition.Arguments.Set(argument.Key, argument.Value);
            condition.StartAction = startAction;
            condition.UnionID = unionId;
            triggerToAddTo.Conditions.Add(condition);

            return condition;
        }
        public static MegaloCondition SetMegaloCondition(string conditionName, MegaloTrigger triggerToAddTo, MegaloAction startAction, KeyValuePair<string, object> argument, int unionId = 0)
        {
            return SetMegaloCondition(conditionName, triggerToAddTo, startAction, new List<KeyValuePair<string, object>> { argument }, unionId);
        }
    }
}
