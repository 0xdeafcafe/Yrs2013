using System.Collections.Generic;
using System.Windows.Forms;
using ModeManager.Twitter;
using System.IO;
using System;
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
                if (i < max)
                {
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

        private void btnCreateAndDeploy_Click(object sender, EventArgs e)
        {
            // Get Base Shit
            Stream templateStfs = new MemoryStream(File.ReadAllBytes(_inputStfsPackage));
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

            // Inject Shit
            stfs.ReplaceFile(tempFile, "variant");

            // Fix Shit
            stfs.Rehash();
            stfs.SaveChanges(@"Stfs/KV_D.bin");

            // Transfer Stuff
            XboxCommunication.XboxCopy(@"Stfs/temp", @"devkit:\Projects\");

            // Tell Shit
            MessageBox.Show("Gametype deployed to console");
        }

        private GameType CreateMegaloGametype(GameType gametype, uint time)
        {
            gametype.TimeLimit = time;
            gametype.Name = new StringTable(string.Format("{0}/{1} - Yrs2013", _cityA, _cityB));

            // Create Megalo Script
            gametype.Script.GlobalVariables.Integers.Add(new IntegerVariableDefinition(0, NetworkPriority.High));

            var traitIndex = gametype.MegaloTraits.Count;
            gametype.MegaloTraits.Add(new MegaloTraits
            {
                WaypointVisible = HUDVisibility.VisibleToEveryone,
                DamageMultiplier = (Modifier)1.6,
                PlayerScale = (Modifier)2.0,
                JumpHeight = (Modifier)2.0
            });

            var initTrigger = new MegaloTrigger();
            #region trigger_data
            {
                // Set Team for initial sessiona
                if (_session.DataPoints[0].CityARating > _session.DataPoints[0].CityBRating)
                    Megalo.SetMegaloAction("var_operation", initTrigger, new List<KeyValuePair<string, object>>
                    {
                        Megalo.CreateCondition("result", new GenericReference(IntegerReference.ForGlobal(traitIndex))),
                        Megalo.CreateCondition("value", new GenericReference(IntegerReference.ForConstant(0))),
                        Megalo.CreateCondition("operation", OperationType.Set)
                    });
                else if (_session.DataPoints[0].CityARating < _session.DataPoints[0].CityBRating)
                    Megalo.SetMegaloAction("var_operation", initTrigger, new List<KeyValuePair<string, object>>
                    {
                        Megalo.CreateCondition("result", new GenericReference(IntegerReference.ForGlobal(traitIndex))),
                        Megalo.CreateCondition("value", new GenericReference(IntegerReference.ForConstant(1))),
                        Megalo.CreateCondition("operation", OperationType.Set)
                    });
                else
                    Megalo.SetMegaloAction("var_operation", initTrigger, new List<KeyValuePair<string, object>>
                    {
                        Megalo.CreateCondition("result", new GenericReference(IntegerReference.ForGlobal(traitIndex))),
                        Megalo.CreateCondition("value", new GenericReference(IntegerReference.ForConstant(0))),
                        Megalo.CreateCondition("operation", OperationType.Set)
                    });

                gametype.Script.EntryPoints.InitTrigger = initTrigger;
                gametype.Script.Triggers.Add(initTrigger);
            }
            #endregion

            var setTraits = new MegaloTrigger { EnumType = TriggerEnumType.EnumPlayers };
            #region trigger_data
            {
                Megalo.SetMegaloAction("compare", setTraits, new List<KeyValuePair<string, object>>
                {
                    Megalo.CreateCondition("value1", new GenericReference(TeamReference.ForPlayerOwnerTeam(PlayerVariableType.CurrentPlayer))),
                    Megalo.CreateCondition("value2", new GenericReference(TeamReference.ForGlobal(TeamVariableType.Global))),
                    Megalo.CreateCondition("comparison", ComparisonType.Equal)
                });

                // scaleAction.Arguments.Set("object", ObjectReference.ForTeamMember(TeamVariableType.CurrentTeam, 1));

                gametype.Script.Triggers.Add(setTraits);
            }
            #endregion

            return gametype;
        }
    }
}
