using System;
using System.Collections.Generic;
using System.Threading;
using Microsoft.Data.Sqlite;
using ShopApp.Models;

namespace ShopApp.WebApi.BD
{
    public class DataBase
    {
        private const string TABLE_NAME = "Products";
        private const string CONNECTION_STRING = "Data Source=shopapp.db;";
        private static readonly Mutex mutex = new Mutex();

        private static readonly string CREATE_TABLE_QUERY =
            $"CREATE TABLE IF NOT EXISTS {TABLE_NAME} (" +
            "Id TEXT PRIMARY KEY, " +
            "Description TEXT, " +
            "Price REAL" +
            ");";

        private static readonly string CREATE_INDEX_QUERY = $"CREATE INDEX IF NOT EXISTS idx_{TABLE_NAME.ToLower()}_id ON {TABLE_NAME} (Id);";

        private static readonly string SELECT_QUERY = $"SELECT Id, Description, Price FROM {TABLE_NAME};";

        private static readonly string INSERT_QUERY = $"INSERT INTO {TABLE_NAME} (Id, Description, Price) VALUES (@Id, @Description, @Price);";

        private static readonly string UPDATE_QUERY = $"UPDATE {TABLE_NAME} SET Description = @Description, Price = @Price WHERE Id = @Id;";

        private static readonly string DELETE_QUERY = $"DELETE FROM {TABLE_NAME} WHERE Id = @Id;";

        public DataBase()
        {
            ExecuteNonQuery(CREATE_TABLE_QUERY);
            ExecuteNonQuery(CREATE_INDEX_QUERY);
        }

        private void ExecuteNonQuery(string query)
        {
            mutex.WaitOne();
            try
            {
                using (var connection = new SqliteConnection(CONNECTION_STRING))
                {
                    connection.Open();
                    using (var command = new SqliteCommand(query, connection))
                    {
                        command.ExecuteNonQuery();
                    }
                }
            }
            finally
            {
                mutex.ReleaseMutex();
            }
        }

        /// <summary>
        /// Возвращает список продуктов из базы данных.
        /// </summary>
        public List<Product> GetProducts()
        {
            var products = new List<Product>();
            mutex.WaitOne();
            try
            {
                using (var connection = new SqliteConnection(CONNECTION_STRING))
                {
                    connection.Open();
                    using (var command = new SqliteCommand(SELECT_QUERY, connection))
                    using (var reader = command.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            products.Add(new Product
                            {
                                Id = Guid.Parse(reader["Id"].ToString()),
                                Description = reader["Description"].ToString(),
                                Price = Convert.ToDouble(reader["Price"])
                            });
                        }
                    }
                }
            }
            finally
            {
                mutex.ReleaseMutex();
            }
            return products;
        }

        /// <summary>
        /// Вставка нового продукта с использованием связывания переменных.
        /// </summary>
        public void InsertProduct(Product product)
        {
            mutex.WaitOne();
            try
            {
                using (var connection = new SqliteConnection(CONNECTION_STRING))
                {
                    connection.Open();
                    using (var command = new SqliteCommand(INSERT_QUERY, connection))
                    {
                        command.Parameters.AddWithValue("@Id", product.Id.ToString());
                        command.Parameters.AddWithValue("@Description", product.Description);
                        command.Parameters.AddWithValue("@Price", product.Price);
                        command.ExecuteNonQuery();
                    }
                }
            }
            finally
            {
                mutex.ReleaseMutex();
            }
        }

        /// <summary>
        /// Обновление существующего продукта.
        /// </summary>
        public void UpdateProduct(Product product)
        {
            mutex.WaitOne();
            try
            {
                using (var connection = new SqliteConnection(CONNECTION_STRING))
                {
                    connection.Open();
                    using (var command = new SqliteCommand(UPDATE_QUERY, connection))
                    {
                        command.Parameters.AddWithValue("@Id", product.Id.ToString());
                        command.Parameters.AddWithValue("@Description", product.Description);
                        command.Parameters.AddWithValue("@Price", product.Price);
                        command.ExecuteNonQuery();
                    }
                }
            }
            finally
            {
                mutex.ReleaseMutex();
            }
        }

        /// <summary>
        /// Удаление продукта по его Id.
        /// </summary>
        public void DeleteProduct(Guid id)
        {
            mutex.WaitOne();
            try
            {
                using (var connection = new SqliteConnection(CONNECTION_STRING))
                {
                    connection.Open();
                    using (var command = new SqliteCommand(DELETE_QUERY, connection))
                    {
                        command.Parameters.AddWithValue("@Id", id.ToString());
                        command.ExecuteNonQuery();
                    }
                }
            }
            finally
            {
                mutex.ReleaseMutex();
            }
        }
    }
}