using System;
using System.IO;
using System.Linq;
using Microsoft.Data.Sqlite;
using NUnit.Framework;
using ShopApp.Models;
using ShopApp.WebApi.BD;

namespace ShopAppTests.BD
{
    [TestFixture]
    public class DataBaseTests
    {
        private const string TestDbFile = "shopapp.db";
        private const string CONNECTION_STRING = "Data Source=shopapp.db;";

        [SetUp]
        public void Setup()
        {
            CleanupDatabase();
        }

        [TearDown]
        public void Teardown()
        {
            CleanupDatabase();
        }

        private void CleanupDatabase()
        {
            SqliteConnection.ClearAllPools();

            if (File.Exists(TestDbFile))
            {
                File.Delete(TestDbFile);
            }
        }

        [Test]
        public void Constructor_ShouldCreateTableAndIndex()
        {
            var db = new DataBase();

            using (var connection = new SqliteConnection(CONNECTION_STRING))
            {
                connection.Open();

                var command = new SqliteCommand(
                    "SELECT name FROM sqlite_master WHERE type='table' AND name='Products';",
                    connection
                );
                var result = command.ExecuteScalar();
                Assert.Multiple(() =>
                {
                    Assert.AreEqual("Products", result, "Table 'Products' was not created.");
                    command.CommandText = "SELECT name FROM sqlite_master WHERE type='index' AND name='idx_products_id';";
                    result = command.ExecuteScalar();
                    Assert.AreEqual("idx_products_id", result, "Index 'idx_products_id' was not created.");
                });
            }
        }

        [Test]
        public void GetProducts_ShouldReturnEmptyList_WhenDatabaseEmpty()
        {
            var db = new DataBase();
            var result = db.GetProducts();

            Assert.AreEqual(0, result.Count, "Expected an empty product list.");
        }

        [Test]
        public void InsertProduct_ShouldAddProductToDatabase()
        {
            var db = new DataBase();
            var product = new Product
            {
                Id = Guid.NewGuid(),
                Description = "Test Product",
                Price = 99.99
            };

            db.InsertProduct(product);

            var result = db.GetProducts();
            Assert.Multiple(() =>
            {
                Assert.AreEqual(1, result.Count, "Product was not added to the database.");
                var insertedProduct = result.First();
                Assert.AreEqual(product.Id, insertedProduct.Id, "Inserted product ID does not match.");
                Assert.AreEqual(product.Description, insertedProduct.Description, "Inserted product description does not match.");
                Assert.AreEqual(product.Price, insertedProduct.Price, "Inserted product price does not match.");
            });
        }

        [Test]
        public void UpdateProduct_ShouldModifyExistingProduct()
        {
            var db = new DataBase();
            var original = new Product
            {
                Id = Guid.NewGuid(),
                Description = "Original",
                Price = 50.0
            };
            db.InsertProduct(original);

            var updated = new Product
            {
                Id = original.Id,
                Description = "Updated",
                Price = 75.0
            };

            db.UpdateProduct(updated);

            var result = db.GetProducts().First();
            Assert.Multiple(() =>
            {
                Assert.AreEqual("Updated", result.Description, "Product description was not updated.");
                Assert.AreEqual(75.0, result.Price, "Product price was not updated.");
            });
        }

        [Test]
        public void DeleteProduct_ShouldRemoveProductFromDatabase()
        {
            var db = new DataBase();
            var product = new Product
            {
                Id = Guid.NewGuid(),
                Description = "To Delete",
                Price = 10.0
            };
            db.InsertProduct(product);
            db.DeleteProduct(product.Id);

            Assert.AreEqual(0, db.GetProducts().Count, "Product was not removed from the database.");
        }

        [Test]
        public void GetProducts_ShouldReturnMultipleProducts()
        {
            var db = new DataBase();
            var products = new[]
            {
                new Product { Id = Guid.NewGuid(), Description = "First", Price = 1.0 },
                new Product { Id = Guid.NewGuid(), Description = "Second", Price = 2.0 }
            };

            foreach (var product in products)
            {
                db.InsertProduct(product);
            }

            var result = db.GetProducts();

            Assert.Multiple(() =>
            {
                Assert.AreEqual(2, result.Count, "Not all products were added to the database.");
                CollectionAssert.AllItemsAreUnique(result, "Duplicate products found in the database.");
                Assert.IsTrue(result.Any(p => p.Description == "First"), "First product not found.");
                Assert.IsTrue(result.Any(p => p.Description == "Second"), "Second product not found.");
            });
        }
    }
}