using System.Collections.Generic;
using System.Windows.Forms;
using ModeManager.Twitter;
using System.IO;
using System;
using Newtonsoft.Json;
using VelocityNet.Stfs;
using Nitrogen.Content.UserGenerated.Halo4;
using Nitrogen.Content.UserGenerated.Halo4.Objects;
using Nitrogen.Content.UserGenerated.Halo4.Megalo;
using Nitrogen.Content.UserGenerated.Halo4.Enums;
using Nitrogen.Content.UserGenerated.Halo4.Traits;

namespace ModeManager
{
    public partial class Form1 : Form
    {
        private readonly string _inputStfsPackage;

        private string _cityA;
        private string _cityB;
        private Session _session;

        public Form1()
        {
            InitializeComponent();
            _inputStfsPackage = "Stfs/gfaiutzayvy545555iuug2vmslaqbaaaaaaaaaaaa";
        }

        private void btnGetCityNames_Click(object sender, EventArgs e)
        {
            // TODO: Read this from memory
            txtCityA.Text = _cityA = "London";
            txtCityB.Text = _cityB = "New York";

            MessageBox.Show("Loaded Cities");
        }

        private void btnDoTwitterAnalysis_Click(object sender, EventArgs e)
        {
            var timeInSeconds = Convert.ToInt32(txtGametypeLength.Text) * 60;
            var twitterStreamer = new Streamer(_cityA, _cityB);
            var i = 0;
            var max = timeInSeconds / 5;
            _session = new Session
            {
                CityA = _cityA,
                CityB = _cityB
            };

            var timer = new Timer { Interval = 5000 };
            timer.Tick += (o, args) =>
            {
                if (i > max)
                {
                    File.WriteAllText(Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory) + "/city_data.json.new", JsonConvert.SerializeObject(_session));

                    // finishing up code
                    timer.Stop();
                    MessageBox.Show("Tweets Analysis Done");
                }
                else
                {
                    var response = twitterStreamer.CalculateShittyCity();
                    _session.DataPoints.Add(new DataPoint
                    {
                        CityARating = response.Item1,
                        CityBRating = response.Item2
                    });
                }

                i++;
            };
            timer.Start();
        }

        private void btnLoadJsonFromLocal_Click(object sender, EventArgs e)
        {
            var ofd = new OpenFileDialog();
            if (ofd.ShowDialog() != DialogResult.OK) return;

            _session = JsonConvert.DeserializeObject<Session>(File.ReadAllText(ofd.FileName));
            MessageBox.Show("Loaded Local JSON");
        }

        private void btnCreateAndDeploy_Click(object sender, EventArgs e)
        {
            var tempStfsFile = Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory) + "\\gfaiutzayvy545555iuug2vmslaqbaaaaaaaaaaaa";
            File.Copy(_inputStfsPackage, tempStfsFile, true);

            // Get Base Shit
            Stream templateStfs = new FileStream(tempStfsFile, FileMode.OpenOrCreate, FileAccess.ReadWrite);
            var stfs = new StfsPackage(templateStfs)
            {
                Name = string.Format("{0}/{1} - Yrs2013", _cityA, _cityB)
            };
            stfs.SaveMetadata();

            // Extract shit
            var tempFile = Path.GetTempFileName();
            stfs.ExtractFile("variant", tempFile);

            // Load Shit
            var gametype = GameType.Load(tempFile);
            gametype = CreateMegaloGametype(gametype, Convert.ToUInt32(txtGametypeLength.Text));

            // Save Shit
            gametype.Save(tempFile);
            if (File.Exists(Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory) + "\\variant"))
                File.Delete(Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory) + "\\variant");
            File.Copy(tempFile, Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory) + "\\variant");

            // Inject Shit
            stfs.ReplaceFile(tempFile, "variant");

            // Fix Shit
            stfs.Rehash();
            stfs.SaveChanges(@"Stfs/KV_D.bin");
            templateStfs.Close();

            // Transfer Stuff
            XboxCommunication.XboxCopy(tempStfsFile, @"hdd:\Content\E00006B45A52FF20\4D530919\00000001");

            // Tell Shit
            MessageBox.Show("Gametype deployed to console");
        }

        private GameType CreateMegaloGametype(GameType gametype, uint time)
        {
            // heuheuheuheuheu
            gametype.TimeLimit = time - 1;

            // string table her dongle so hard
            gametype.Name = new StringTable(string.Format("{0}/{1} - Yrs2013", _cityA, _cityB));

            // setup teams
            #region setup teams
            gametype.TeamsEnabled = true;
            gametype.TeamChanging = TeamChangingMode.Disabled;
            #endregion
            #region global variable declaration
            var currentTeamIndex = gametype.Script.GlobalVariables.Integers.Count;
            gametype.Script.GlobalVariables.Integers.Add(new IntegerVariableDefinition(0, NetworkPriority.High));

            var currentDataPointIndex = gametype.Script.GlobalVariables.Integers.Count;
            gametype.Script.GlobalVariables.Integers.Add(new IntegerVariableDefinition(0, NetworkPriority.High));

            var updateNextDataPoint = gametype.Script.GlobalVariables.Integers.Count;
            gametype.Script.GlobalVariables.Integers.Add(new IntegerVariableDefinition(1, NetworkPriority.High));

            var updateTimerIndex = gametype.Script.GlobalVariables.Timers.Count;
            gametype.Script.GlobalVariables.Timers.Add(new TimerVariableDefinition(0));
            #endregion
            #region trait creation
            var traitIndex = gametype.MegaloTraits.Count;
            gametype.MegaloTraits.Add(new MegaloTraits
            {
                WaypointVisible = HUDVisibility.VisibleToEveryone,
                DamageMultiplier = (Modifier)1.6,
                PlayerScale = (Modifier)2.0,
                JumpHeight = (Modifier)2.0
            });
            #endregion

            // ************************************************************************************
            // To All Megalo Script Under Here ****************************************************
            // ************************************************************************************

            var initTrigger = new MegaloTrigger();
            #region trigger_data
            {
                Megalo.SetMegaloAction("var_operation", initTrigger,
                    new List<KeyValuePair<string, object>>
                    {
                        Megalo.CreateCondition("result", new GenericReference(TimerReference.ForGlobal(updateTimerIndex))),
                        Megalo.CreateCondition("value", new GenericReference(IntegerReference.ForConstant(5))),
                        Megalo.CreateCondition("operation", OperationType.Set)
                    });

                Megalo.SetMegaloAction("timer_set_rate", initTrigger,
                    new List<KeyValuePair<string, object>>
                    {
                        Megalo.CreateCondition("timer", TimerReference.ForGlobal(updateTimerIndex)),
                        Megalo.CreateCondition("rate", 5)
                    });

                gametype.Script.EntryPoints.InitTrigger = initTrigger;
                gametype.Script.Triggers.Add(initTrigger);
            }
            #endregion

            var trySetTraitsToTeam0 = new MegaloTrigger { EnumType = TriggerEnumType.EnumPlayers };
            #region trigger_data
            {
                var setPlayerTraits = Megalo.SetMegaloAction("player_set_traits", trySetTraitsToTeam0, new List<KeyValuePair<string, object>>
                {
                    Megalo.CreateCondition("player", PlayerReference.ForGlobal(PlayerVariableType.CurrentPlayer)),
                    Megalo.CreateCondition("traits_index", traitIndex)
                });

                Megalo.SetMegaloCondition("compare", trySetTraitsToTeam0, setPlayerTraits, new List<KeyValuePair<string, object>>
                {
                    Megalo.CreateCondition("value1", new GenericReference(IntegerReference.ForGlobal(currentTeamIndex))),
                    Megalo.CreateCondition("value2", new GenericReference(IntegerReference.ForConstant(0))),
                    Megalo.CreateCondition("comparison", ComparisonType.Equal)
                });

                Megalo.SetMegaloCondition("compare", trySetTraitsToTeam0, setPlayerTraits, new List<KeyValuePair<string, object>>
                {
                    Megalo.CreateCondition("value1", new GenericReference(TeamReference.ForPlayerOwnerTeam(PlayerVariableType.CurrentPlayer))),
                    Megalo.CreateCondition("value2", new GenericReference(TeamReference.ForGlobal(TeamVariableType.Direct, 0))),
                    Megalo.CreateCondition("comparison", ComparisonType.Equal)
                }, 1);

                gametype.Script.Triggers.Add(trySetTraitsToTeam0);
            }
            #endregion

            var trySetTraitsToTeam1 = new MegaloTrigger { EnumType = TriggerEnumType.EnumPlayers };
            #region trigger_data
            {
                var setPlayerTraits = Megalo.SetMegaloAction("player_set_traits", trySetTraitsToTeam1, new List<KeyValuePair<string, object>>
                {
                    Megalo.CreateCondition("player", PlayerReference.ForGlobal(PlayerVariableType.CurrentPlayer)),
                    Megalo.CreateCondition("traits_index", traitIndex)
                });

                Megalo.SetMegaloCondition("compare", trySetTraitsToTeam1, setPlayerTraits, new List<KeyValuePair<string, object>>
                {
                    Megalo.CreateCondition("value1", new GenericReference(IntegerReference.ForGlobal(currentTeamIndex))),
                    Megalo.CreateCondition("value2", new GenericReference(IntegerReference.ForConstant(1))),
                    Megalo.CreateCondition("comparison", ComparisonType.Equal)
                });

                Megalo.SetMegaloCondition("compare", trySetTraitsToTeam1, setPlayerTraits, new List<KeyValuePair<string, object>>
                {
                    Megalo.CreateCondition("value1", new GenericReference(TeamReference.ForPlayerOwnerTeam(PlayerVariableType.CurrentPlayer))),
                    Megalo.CreateCondition("value2", new GenericReference(TeamReference.ForGlobal(TeamVariableType.Direct, 1))),
                    Megalo.CreateCondition("comparison", ComparisonType.Equal)
                }, 1);

                gametype.Script.Triggers.Add(trySetTraitsToTeam1);
            }
            #endregion

            var updateDataPointEntryTrigger = new MegaloTrigger();
            #region trigger_data
            {
                // start action
                var resetNeedUpdateDataPoint = Megalo.SetMegaloAction("var_operation", updateDataPointEntryTrigger,
                    new List<KeyValuePair<string, object>>
                    {
                        Megalo.CreateCondition("result", new GenericReference(IntegerReference.ForGlobal(updateNextDataPoint))),
                        Megalo.CreateCondition("value", new GenericReference(IntegerReference.ForConstant(0))),
                        Megalo.CreateCondition("operation", OperationType.Set)
                    });

                var index = 0;
                foreach (var dataPoint in _session.DataPoints)
                {
                    var constructedMegaloTrigger = new MegaloTrigger();
                    {
                        MegaloAction startAction;
                        var winner = "";
                        if (dataPoint.CityARating > dataPoint.CityBRating)
                        {
                            winner = _session.CityA;

                            startAction = Megalo.SetMegaloAction("var_operation", constructedMegaloTrigger,
                                new List<KeyValuePair<string, object>>
                                {
                                    Megalo.CreateCondition("result",
                                        new GenericReference(IntegerReference.ForGlobal(currentTeamIndex))),
                                    Megalo.CreateCondition("value",
                                        new GenericReference(IntegerReference.ForConstant(0))),
                                    Megalo.CreateCondition("operation", OperationType.Set)
                                });
                        }
                        else if (dataPoint.CityARating < dataPoint.CityBRating)
                        {
                            winner = _session.CityB;

                            startAction = Megalo.SetMegaloAction("var_operation", constructedMegaloTrigger,
                                new List<KeyValuePair<string, object>>
                                {
                                    Megalo.CreateCondition("result",
                                        new GenericReference(IntegerReference.ForGlobal(currentTeamIndex))),
                                    Megalo.CreateCondition("value",
                                        new GenericReference(IntegerReference.ForConstant(1))),
                                    Megalo.CreateCondition("operation", OperationType.Set)
                                });
                        }
                        else
                        {
                            winner = _session.CityA;

                            startAction = Megalo.SetMegaloAction("var_operation", constructedMegaloTrigger,
                                new List<KeyValuePair<string, object>>
                                {
                                    Megalo.CreateCondition("result",
                                        new GenericReference(IntegerReference.ForGlobal(currentTeamIndex))),
                                    Megalo.CreateCondition("value",
                                        new GenericReference(IntegerReference.ForConstant(0))),
                                    Megalo.CreateCondition("operation", OperationType.Set)
                                });
                        }



                        var str = new StringReference(gametype.MegaloStrings.Add(string.Format("Current Winner Is: {0}!", winner)));
                        var effectMessage = new MegaloAction("chud_message");
                        effectMessage.Arguments.Set("target", TargetReference.All);
                        effectMessage.Arguments.Set("sound_index", -1);
                        effectMessage.Arguments.Set("text", str);
                        constructedMegaloTrigger.Actions.Add(effectMessage);

                        // call if swag is gucci
                        Megalo.SetMegaloCondition("compare", constructedMegaloTrigger, startAction,
                            new List<KeyValuePair<string, object>>
                            {
                                Megalo.CreateCondition("value1", new GenericReference(IntegerReference.ForGlobal(currentDataPointIndex))),
                                Megalo.CreateCondition("value2", new GenericReference(IntegerReference.ForConstant((short)index))),
                                Megalo.CreateCondition("comparison", ComparisonType.Equal)
                            });
                    }
                    var callConstructedMegaloTrigger = MegaloAction.ForTriggerCall(constructedMegaloTrigger);
                    updateDataPointEntryTrigger.Actions.Add(callConstructedMegaloTrigger);

                    index++;
                }

                Megalo.SetMegaloAction("var_operation", updateDataPointEntryTrigger,
                    new List<KeyValuePair<string, object>>
                    {
                        Megalo.CreateCondition("result", new GenericReference(IntegerReference.ForGlobal(currentDataPointIndex))),
                        Megalo.CreateCondition("value", new GenericReference(IntegerReference.ForConstant(1))),
                        Megalo.CreateCondition("operation", OperationType.Add)
                    });

                Megalo.SetMegaloCondition("compare", updateDataPointEntryTrigger, resetNeedUpdateDataPoint,
                    new List<KeyValuePair<string, object>>
                    {
                        Megalo.CreateCondition("value1", new GenericReference(IntegerReference.ForGlobal(updateNextDataPoint))),
                        Megalo.CreateCondition("value2", new GenericReference(IntegerReference.ForConstant(1))),
                        Megalo.CreateCondition("comparison", ComparisonType.Equal)
                    });

                gametype.Script.Triggers.Add(updateDataPointEntryTrigger);
            }
            #endregion

            var dealWithTheTimerTrigger = new MegaloTrigger();
            #region trigger_data
            {
                // check timer has expired, if it has, set a variable and start it again
                var setTimer = Megalo.SetMegaloAction("var_operation", dealWithTheTimerTrigger,
                    new List<KeyValuePair<string, object>>
                    {
                        Megalo.CreateCondition("result", new GenericReference(TimerReference.ForGlobal(updateTimerIndex))),
                        Megalo.CreateCondition("value", new GenericReference(IntegerReference.ForConstant(3))),
                        Megalo.CreateCondition("operation", OperationType.Set)
                    });

                Megalo.SetMegaloAction("timer_set_rate", dealWithTheTimerTrigger,
                    new List<KeyValuePair<string, object>>
                    {
                        Megalo.CreateCondition("timer", TimerReference.ForGlobal(updateTimerIndex)),
                        Megalo.CreateCondition("rate", 5)
                    });

                // set variable
                Megalo.SetMegaloAction("var_operation", dealWithTheTimerTrigger,
                    new List<KeyValuePair<string, object>>
                    {
                        Megalo.CreateCondition("result", new GenericReference(IntegerReference.ForGlobal(updateNextDataPoint))),
                        Megalo.CreateCondition("value", new GenericReference(IntegerReference.ForConstant(1))),
                        Megalo.CreateCondition("operation", OperationType.Set)
                    });

                Megalo.SetMegaloCondition("timer_is_zero", dealWithTheTimerTrigger, setTimer,
                    new List<KeyValuePair<string, object>>
                    {
                        Megalo.CreateCondition("timer", TimerReference.ForGlobal(updateTimerIndex))
                    });

                gametype.Script.Triggers.Add(dealWithTheTimerTrigger);
            }
            #endregion

            var roundEndingTimer = new MegaloTrigger();
            #region trigger_data
            {
                var endRound = Megalo.SetMegaloAction("round_end", roundEndingTimer);

                Megalo.SetMegaloCondition("timer_is_zero", roundEndingTimer, endRound,
                    new List<KeyValuePair<string, object>>
                    {
                        Megalo.CreateCondition("timer", TimerReference.RoundTimer)
                    });

                gametype.Script.Triggers.Add(roundEndingTimer);
            }
            #endregion

            return gametype;
        }
    }
}
